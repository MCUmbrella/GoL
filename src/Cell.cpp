//
// Created by mcumbrella on 23-5-10.
//

#include "Cell.h"
#include "GoL.h"

std::string Cell::toString() const
{
#ifdef _WIN32
    return state == STATE_BORDER ? "##" :
           state == STATE_DEAD ? ". "
                               : "[]";
#else
    return state == STATE_BORDER ? "囗" :
           state == STATE_DEAD ? "丶"
                               : "回";
#endif
}

Cell::Cell(const int& line, const int& row, const CellState& state)
{
    this->line = line;
    this->row = row;
    this->state = state;
    this->nextState = state;
}

CellState Cell::getState() const
{
    return state;
}

void Cell::setState(const CellState& newState)
{
    state = newState;
}

CellState Cell::getNextState() const
{
    return nextState;
}

void Cell::setNextState(const CellState& newState)
{
    nextState = newState;
}

char Cell::toChar() const
{
    return state == STATE_BORDER ? '#' :
           state == STATE_DEAD ? '0' :
           '1';
}

CellState Cell::calculateNextState() const
{
    if(state == STATE_BORDER) return STATE_BORDER;
    int neighbours = 0;
    GoL& app = GoL::getInstance();
    if (app.getStateOf(line - 1, row - 1) == STATE_ALIVE) ++neighbours;
    if (app.getStateOf(line - 1, row) == STATE_ALIVE) ++neighbours;
    if (app.getStateOf(line - 1, row + 1) == STATE_ALIVE) ++neighbours;
    if (app.getStateOf(line, row - 1) == STATE_ALIVE) ++neighbours;
    if (app.getStateOf(line, row + 1) == STATE_ALIVE) ++neighbours;
    if (app.getStateOf(line + 1, row - 1) == STATE_ALIVE) ++neighbours;
    if (app.getStateOf(line + 1, row) == STATE_ALIVE) ++neighbours;
    if (app.getStateOf(line + 1, row + 1) == STATE_ALIVE) ++neighbours;
    return neighbours == 3 || (state == STATE_ALIVE && neighbours == 2) ? STATE_ALIVE : STATE_DEAD;
}
