//
// Created by mcumbrella on 23-5-10.
//

#include <fstream>
#include <sstream>
#include "GoL.h"
#include "Cell.h"
#include "CommonUtil.h"

#define t CommonUtil::transparent

using namespace std;

GoL& GoL::getInstance()
{
    static GoL instance;
    return instance;
}

GoL& GoL::init(const int& initLines, const int& initRows)
{
    if (initLines < 2 || initRows < 2) throw runtime_error("Line number and row number must be >= 2");

    // add a border with the width of 1 cell
    lines = initLines + 2;
    rows = initRows + 2;

    cout << "Initializing cell board with size " << initRows << " * " << initLines << endl;
    cells = vector<vector<Cell>>(lines);
    for (int i = 0; i != lines; ++i)
        for (int j = 0; j != rows; ++j)
            if (i == 0 || i == lines - 1)
                cells[i].emplace_back(i, j, STATE_BORDER);
            else
                cells[i].emplace_back(i, j, j == 0 || j == rows - 1 ? STATE_BORDER : STATE_DEAD);

    cacheCellNeighbours();

    cout << "Cell board initialization completed" << endl;
    return *this;
}

GoL& GoL::init(const string& initFilePath)
{
    cout << "Using input file: " << initFilePath << endl;
    ifstream in(initFilePath);
    if (!in) throw runtime_error(string("Unable to read input file: ").append(initFilePath));

    // read line numbers and row numbers from input file
    int l = 0, r = 0;
    in >> l >> r;
    // initialize cells
    init(l, r);

    // read the pattern from the input file
    cout << "Loading pattern from input" << endl;
    string line;
    for (int i = 0; i != getLines(); ++i)
    {
        if (in >> line)
        {
            if (line.length() != getRows())
            {
                stringstream msg;
                msg << "Line length mismatch: at line " << i + 1 << " expected " << getRows() << " but got " << line.length();
                throw runtime_error(msg.str());
            }
            for (int j = 0; j != getRows(); ++j)
                setStateOf(i + 1, j + 1, CommonUtil::parseCellState(line[j]));
        }
        else
        {
            stringstream msg;
            msg << "Total line number mismatch: expected " << getLines() << " but got " << i;
            throw runtime_error(msg.str());
        }
    }
    cout << "Pattern setup completed" << endl;
    in.close();
    cout << "Initialization completed" << endl;
    return *this;
}

GoL& GoL::save(const string& filePath)
{
    ofstream out(filePath);
    if (!out.bad())
    {
        out << getLines() << ' ' << getRows() << endl;
        for (int i = 1; i <= getLines(); ++i)
        {
            for (int j = 1; j <= getRows(); ++j)
                out << cells[i][j].toChar();
            out << endl;
        }
        out.close();
    }
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
    for (int i = 1; i <= getLines(); ++i)
    {
        for (int j = 1; j <= getRows(); ++j)
        {
            Cell& c = getCell(i, j);
            c.setNextState(c.calculateNextState());
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

void GoL::cacheCellNeighbours()
{
    if (flNoBorder)
        for (int i = 1; i <= getLines(); ++i)
            for (int j = 1; j <= getRows(); ++j)
            {
                Cell& c = cells[i][j];
                c.setNeighbour(&(cells[t(i - 1, getLines())][t(j - 1, getRows())]))
                 .setNeighbour(&(cells[t(i - 1, getLines())][t(j, getRows())]))
                 .setNeighbour(&(cells[t(i - 1, getLines())][t(j + 1, getRows())]))
                 .setNeighbour(&(cells[t(i, getLines())][t(j - 1, getRows())]))
                 .setNeighbour(&(cells[t(i, getLines())][t(j + 1, getRows())]))
                 .setNeighbour(&(cells[t(i + 1, getLines())][t(j - 1, getRows())]))
                 .setNeighbour(&(cells[t(i + 1, getLines())][t(j, getRows())]))
                 .setNeighbour(&(cells[t(i + 1, getLines())][t(j + 1, getRows())]));
            }
    else
        for (int i = 1; i <= getLines(); ++i)
            for (int j = 1; j <= getRows(); ++j)
            {
                Cell& c = cells[i][j];
                c.setNeighbour(&(cells[i - 1][j - 1]))
                 .setNeighbour(&(cells[i - 1][j]))
                 .setNeighbour(&(cells[i - 1][j + 1]))
                 .setNeighbour(&(cells[i][j - 1]))
                 .setNeighbour(&(cells[i][j + 1]))
                 .setNeighbour(&(cells[i + 1][j - 1]))
                 .setNeighbour(&(cells[i + 1][j]))
                 .setNeighbour(&(cells[i + 1][j + 1]));
            }
}

GoL& GoL::display(const bool& border)
{
    if (border)
        for (int i = 0; i != lines; ++i)
        {
            for (int j = 0; j != rows; ++j)
                cout << cells[i][j].toString();
            cout << endl;
        }
    else
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

bool GoL::isNoBorder() const
{
    return flNoBorder;
}

Cell& GoL::getCell(const int& line, const int& row)
{
    if (flNoBorder)
        return cells[t(line, getLines())][t(row, getRows())];

    if ((line < 1 || line > getLines()) || (row < 1 || row > getRows()))
        throw out_of_range("Location out of bounds");
    return cells[line][row];
}

CellState GoL::getStateOf(const int& line, const int& row)
{
    if (flNoBorder)
        return cells[t(line, getLines())][t(row, getRows())].getState();

    if ((line < 1 || line > getLines()) || (row < 1 || row > getRows()))
        return STATE_BORDER;
    return cells[line][row].getState();
}

void GoL::setStateOf(const int& line, const int& row, CellState state)
{
    if (flNoBorder)
        cells[t(line, getLines())][t(row, getRows())].setState(state);

    if ((line < 1 || line > getLines()) || (row < 1 || row > getRows()))
        throw out_of_range("Location out of bounds");
    cells[line][row].setState(state);
}

GoL& GoL::toggleBorder(const bool& status)
{
    flNoBorder = status;
    return *this;
}

GoL& GoL::revert(const int& steps)
{
    if (steps < 1) return *this;
    for (int i = 0; i != steps; ++i)
    {
        if (previousCells.empty())
            break;
        cells = vector<vector<Cell>>(previousCells.top());
        previousCells.pop();
        --currentGeneration;
    }
    cacheCellNeighbours(); // remake neighbour cache after reverting to avoid some wild pointer issues
    return *this;
}

GoL& GoL::forward(const int& steps)
{
    if (steps > 0)
        for (int i = 0; i != steps; ++i)
            run();
    return *this;
}
