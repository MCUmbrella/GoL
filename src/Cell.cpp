//
// Created by mcumbrella on 23-5-10.
//

#include "Cell.h"

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

Cell::Cell(const CellState& state)
{
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
