//
// Created by mcumbrella on 23-5-10.
//

#ifndef GOL_GOL_H
#define GOL_GOL_H

#include <iostream>
#include <vector>
#include "Cell.h"

class GoL
{
private:
    int currentGeneration = 0, rows = 0, lines = 0;
    std::vector<std::vector<Cell>> cells;
    std::vector<std::vector<Cell>> previousCells;

    GoL() = default;

    ~GoL() = default;

    void calculateNextGeneration();

    void applyNextGeneration();

public:
    GoL(const GoL&) = delete;

    GoL& operator =(const GoL&) = delete;

    static GoL& getInstance();

    GoL& init(const std::string& initFilePath);

    GoL& run();

    GoL& display();

    int getCurrentGeneration() const;

    int getLines() const;

    int getRows() const;

    Cell& getCell(const int& line, const int& row);

    CellState getStateOf(const int& line, const int& row);

    void setStateOf(const int& line, const int& row, CellState state);
};

#endif //GOL_GOL_H
