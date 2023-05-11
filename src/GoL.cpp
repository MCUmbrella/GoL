//
// Created by mcumbrella on 23-5-10.
//

#include <fstream>
#include "GoL.h"
#include "Cell.h"
#include "CommonUtil.h"

using namespace std;

GoL& GoL::getInstance()
{
    static GoL instance;
    return instance;
}

GoL& GoL::init(const string& initFilePath)
{
    cout << "Using input file: " << initFilePath << endl;
    ifstream in(initFilePath);
    if (!in) throw runtime_error(string("Unable to read input file: ").append(initFilePath));
    // read line numbers and row numbers from input file
    in >> lines;
    in >> rows;
    // add a border with the width of 1 cell
    lines += 2;
    rows += 2;

    // initialize cells
    if (lines < 4 || rows < 4) throw runtime_error("Invalid line number or row number");
    cout << "Initializing cell board with size " << rows - 2 << " * " << lines - 2 << endl;
    cells = vector<vector<Cell>>(lines);
    for (int i = 0; i != lines; ++i)
    {
        cells[i] = vector<Cell>(rows);
        for (int j = 1; j != rows - 1; ++j)
            if (i != 0 && i <= getLines())
                cells[i][j].setState(STATE_DEAD);
    }
    cout << "Cell board initialization completed" << endl;

    // read the pattern from the input file
    cout << "Loading pattern from input" << endl;
    string line;
    for (int i = 0; i != getLines(); ++i)
    {
        if (in >> line)
        {
            if (line.length() != getRows())
                throw runtime_error(
                        string("Line length mismatch: expected ").append(to_string(getRows()).append(" but got ").append(to_string(line.length())))
                );
            for (int j = 0; j != getRows(); ++j)
                setStateOf(i + 1, j + 1, CommonUtil::parseCellState(line.at(j)));
        }
        else
            throw runtime_error(
                    string("Total line number mismatch: expected ").append(to_string(getLines()).append(" but got ").append(to_string(i)))
            );
    }
    cout << "Pattern setup completed" << endl;
    in.close();
    cout << "Initialization completed" << endl;
    return *this;
}

GoL& GoL::run()
{
    previousCells.push(vector<vector<Cell>>(cells));
    calculateNextGeneration();
    applyNextGeneration();
    ++currentGeneration;
    return *this;
}

void GoL::calculateNextGeneration()
{
    int neighbours;
    for (int i = 1; i <= getLines(); ++i)
    {
        for (int j = 1; j <= getRows(); ++j)
        {
            neighbours = 0;
            Cell& c = getCell(i, j);
            if (getStateOf(i - 1, j - 1) == STATE_ALIVE) ++neighbours;
            if (getStateOf(i - 1, j) == STATE_ALIVE) ++neighbours;
            if (getStateOf(i - 1, j + 1) == STATE_ALIVE) ++neighbours;
            if (getStateOf(i, j - 1) == STATE_ALIVE) ++neighbours;
            if (getStateOf(i, j + 1) == STATE_ALIVE) ++neighbours;
            if (getStateOf(i + 1, j - 1) == STATE_ALIVE) ++neighbours;
            if (getStateOf(i + 1, j) == STATE_ALIVE) ++neighbours;
            if (getStateOf(i + 1, j + 1) == STATE_ALIVE) ++neighbours;
            if (c.getState() == STATE_ALIVE)
            {
                if (neighbours == 2 || neighbours == 3)
                    c.setNextState(STATE_ALIVE);
                else
                    c.setNextState(STATE_DEAD);
            }
            else if (c.getState() == STATE_DEAD)
            {
                if (neighbours == 3)
                    c.setNextState(STATE_ALIVE);
                else
                    c.setNextState(STATE_DEAD);
            }
        }
    }
}

void GoL::applyNextGeneration()
{
    for (int i = 1; i <= getLines(); ++i)
    {
        for (int j = 1; j <= getRows(); ++j)
        {
            Cell& c = getCell(i, j);
            c.setState(c.getNextState());
        }
    }
}

GoL& GoL::display()
{
    for (int i = 1; i <= getLines(); ++i)
    {
        for (int j = 1; j <= getRows(); ++j)
            cout << cells[i][j].toString();
        cout << endl;
    }
    return *this;
}

int GoL::getCurrentGeneration() const
{
    return currentGeneration;
}

int GoL::getLines() const
{
    return lines - 2;
}

int GoL::getRows() const
{
    return rows - 2;
}

Cell& GoL::getCell(const int& line, const int& row)
{
    if (flNoBorder)
        return cells[CommonUtil::transparent(line, getLines())][CommonUtil::transparent(row, getRows())];

    if ((line < 1 || line > getLines()) || (row < 1 || row > getRows()))
        throw out_of_range(string("Location out of bounds"));
    return cells[line][row];
}

CellState GoL::getStateOf(const int& line, const int& row)
{
    if (flNoBorder)
        return cells[CommonUtil::transparent(line, getLines())][CommonUtil::transparent(row, getRows())].getState();

    if ((line < 1 || line > getLines()) || (row < 1 || row > getRows()))
        return STATE_BORDER;
    return cells[line][row].getState();
}

void GoL::setStateOf(const int& line, const int& row, CellState state)
{
    if (flNoBorder)
        cells[CommonUtil::transparent(line, getLines())][CommonUtil::transparent(row, getRows())].setState(state);

    if ((line < 1 || line > getLines()) || (row < 1 || row > getRows()))
        throw out_of_range(string("Location out of bounds"));
    cells[line][row].setState(state);
}

GoL& GoL::toggleBorder(const bool& status)
{
    flNoBorder = status;
    return *this;
}

GoL& GoL::redo(const int& steps)
{
    for (int i = 0; i != steps; i++)
    {
        if (previousCells.empty())
            break;
        cells = vector<vector<Cell>>(previousCells.top());
        previousCells.pop();
        --currentGeneration;
    }
    return *this;
}
