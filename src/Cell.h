//
// Created by mcumbrella on 23-5-10.
//

#ifndef GOL_CELL_H
#define GOL_CELL_H

#include "CellState.h"
#include <string>

/**
 * A cell is the smallest unit in the cell board.
 * Except for the border cells, each cell can be alive or dead,
 * depends on the number of living neighbours.
 */
class Cell
{
private:
    CellState state = STATE_BORDER;
    CellState nextState = STATE_BORDER;
public:
    Cell() = default;

    explicit Cell(const CellState& state);

    CellState getState() const;

    void setState(const CellState& newState);

    CellState getNextState() const;

    void setNextState(const CellState& newState);

    /**
     * Converts the cell to string.
     * @return Square if the cell is alive, dot if the cell is dead.
     * The exact look depends on the operating system.
     */
    std::string toString() const;

    char toChar() const;
};

#endif //GOL_CELL_H
