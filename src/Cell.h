//
// Created by mcumbrella on 23-5-10.
//

#ifndef GOL_CELL_H
#define GOL_CELL_H

#include "CellState.h"
#include <string>

class Cell
{
private:
    CellState state = STATE_BORDER;
    CellState nextState = STATE_BORDER;
public:
    Cell() = default;

    explicit Cell(CellState state);

    CellState getState() const;

    void setState(CellState newState);

    CellState getNextState() const;

    void setNextState(CellState newState);

    std::string toString() const;
};

#endif //GOL_CELL_H
