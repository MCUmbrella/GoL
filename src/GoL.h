//
// Created by mcumbrella on 23-5-10.
//

#ifndef GOL_GOL_H
#define GOL_GOL_H

#include <iostream>
#include <vector>
#include <stack>
#include "Cell.h"

using std::vector;
using std::stack;

/**
 * The Game of Life engine, a singleton application.
 */
class GoL
{
private:
    bool flNoBorder = false;
    int currentGeneration = 0, rows = 0, lines = 0;
    vector<vector<Cell>> cells; // the cell board
    stack<vector<vector<Cell>>> previousCells; // the previous state of the cell board

    GoL() = default;

    ~GoL() = default;

    void calculateNextGeneration();

    void applyNextGeneration();

public:
    GoL(const GoL&) = delete;

    GoL& operator =(const GoL&) = delete;

    /**
     * Gets the instance of the engine.
     */
    static GoL& getInstance();

    /**
     * Initialize the cell board from an input file.
     * @param initFilePath The path of the input file.
     */
    GoL& init(const std::string& initFilePath);

    /**
     * Do a iteration.
     */
    GoL& run();

    /**
     * Prints the current state of the cell board to the terminal.
     * @return
     */
    GoL& display();

    int getCurrentGeneration() const;

    /**
     * How many lines does the cell board have?
     * @return The total lines of the cell board, the border is not included.
     */
    int getLines() const;

    /**
     * How many rows does the cell board have?
     * @return The total rows of the cell board, the border is not included.
     */
    int getRows() const;

    /**
     * Gets the reference of a cell at the specified location.
     * @param line Line number, start from 1.
     * @param row Row number, start from 1.
     */
    Cell& getCell(const int& line, const int& row);

    /**
     * Gets the current state of a cell at the specified location.
     * @param line Line number, start from 1.
     * @param row Row number, start from 1.
     */
    CellState getStateOf(const int& line, const int& row);

    /**
     * Sets the current state of a cell at the specified location.
     * @param line Line number, start from 1.
     * @param row Row number, start from 1.
     * @param state The new state of the cell.
     */
    void setStateOf(const int& line, const int& row, CellState state);

    /**
     * Turns on/off the border.
     * If the border has been turned off, the two sides of the cell board
     * are connected, which means the cells will appear on the opposite
     * side after moving towards the edge.
     * @param status The new border status.
     */
    GoL& toggleBorder(const bool& status);

    GoL& redo(const int& steps);
};

#endif //GOL_GOL_H
