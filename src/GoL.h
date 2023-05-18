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
 * The Game of Life simulation engine, designed with singleton pattern.
 */
class GoL
{
private:
    bool flNoBorder = false;
    int currentGeneration = 0, lines = 0, columns = 0;
    vector<vector<Cell>> cells; // the cell board
    stack<vector<vector<Cell>>> previousCells; // the previous state of the cell board

    GoL() = default;

    ~GoL() = default;

    /**
     * Let each cell calculate and set its next state.
     */
    void calculateNextGeneration();

    /**
     * Let each cell apply its next state and finally let the
     * generation counter +1, i.e., complete an iteration.
     */
    void applyNextGeneration();

    /**
     * Set each (non-border) cell's neighbour cache.
     */
    void cacheCellNeighbours();

public:
    GoL(const GoL&) = delete;

    GoL& operator =(const GoL&) = delete;

    /**
     * Gets the instance of the engine.
     */
    static GoL& getInstance();

    /**
     * Initialize an empty cell board with a specified size
     * @param initLines The lines of the cell board (without border).
     * @param initColumns The columns of the cell board (without border).
     */
    GoL& init(const int& initLines, const int& initColumns);

    /**
     * Initialize the cell board from an input file.
     * @param initFilePath The path of the input file.
     */
    GoL& init(const std::string& initFilePath);

    /**
     * Save the cell board to a local file.
     * @param filePath The path of the file
     */
    GoL& save(const std::string& filePath);

    /**
     * Do an iteration.
     */
    GoL& run();

    /**
     * Prints the current state of the cell board to the terminal.
     * @param border Also print the border when set to true.
     */
    GoL& display(const bool& border);

    int getCurrentGeneration() const;

    /**
     * How many lines does the cell board have?
     * @return The total lines of the cell board, the border is not included.
     */
    int getLines() const;

    /**
     * How many columns does the cell board have?
     * @return The total columns of the cell board, the border is not included.
     */
    int getColumns() const;

    /**
     * Check if the cell board has transparent border enabled.
     */
    bool isNoBorder() const;

    /**
     * Gets the reference of a cell at the specified location.
     * @param line Line number, start from 1.
     * @param column Column number, start from 1.
     */
    Cell& getCell(const int& line, const int& column);

    /**
     * Sets the current state of a cell at the specified location.
     * @param line Line number, start from 1.
     * @param column Column number, start from 1.
     * @param state The new state of the cell.
     */
    void setStateOf(const int& line, const int& column, CellState state);

    /**
     * Turns on/off the border. When the border is turned off, it becomes
     * 'transparent', which means the two sides of the cell board are
     * connected, and the cells will appear on the opposite side after
     * moving towards the edge.
     * @param status The new border status.
     */
    GoL& toggleNoBorder(const bool& status);

    /**
     * Undo a specified amount of iterations.
     */
    GoL& revert(const int& steps);

    /**
     * Do a specified amount of iterations.
     */
    GoL& forward(const int& steps);
};

#endif //GOL_GOL_H
