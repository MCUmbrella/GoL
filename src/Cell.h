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
    int line = 0, row = 0;
    CellState state = STATE_BORDER;
    CellState nextState = STATE_BORDER;
public:
    Cell() = default;

    Cell(const int& line, const int& row, const CellState& state);

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

    /**
     * Converts the state of the cell to a character.
     * @return '1' if alive, '0' if dead, '#' if border.
     */
    char toChar() const;

    /**
     * Calculates the next state of the cell based on the states of the surrounding 8 cells.
     * @return
     * STATE_BORDER if:
     * <ul>
     * <li> the current state of the cell is STATE_BORDER </li>
     * </ul>
     * STATE_ALIVE if:
     * <ul>
     * <li> the cell is surrounded by 3 live cells </li>
     * <li> or the cell is live and surrounded by 2 live cells </li>
     * </ul>
     * STATE_DEAD if:
     * <ul>
     * <li> none of the above conditions are met </li>
     * </ul>
     */
    CellState calculateNextState() const;
};

#endif //GOL_CELL_H
