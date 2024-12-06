#ifndef GRID_HPP
#define GRID_HPP

#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include "cell.hpp"

using namespace std;

class Grid
{
private:
    int gridWidth;
    int gridHeight;
    vector<vector<Cell>> grid;

public:
    Grid();
    Grid(int gridWidth, int gridHeight);

    // Accesseurs
    int getWidth() const { return gridWidth; }
    int getHeight() const { return gridHeight; }

    // Méthodes
    void initializeGrid();
    int getCellNeighbor(int x, int y) const;
    void rule(int x, int y);
    void update();
    void loadGridFromFile(const std::string &filename, bool resizeGrid);
    void Export(const std::string &filename) const;

    // Accesseur non-const pour accéder aux cellules
    Cell &getCell(int x, int y);
};

#endif // GRID_HPP
