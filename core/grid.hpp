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

    int getWidth() {return gridWidth;}
    int getHeight() {return gridHeight;}

    void initializeGrid();
    int getCellNeighbor(int x, int y);
    void rule(int x, int y);
    void update();
    void loadGridFromFile(const std::string &filename, bool resizeGrid);   

    Cell &getCell(int x, int y); 

    void Export(const std::string &filename);
};

#endif // GRID_HPP