#ifndef MAP_H
#define MAP_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <ctime>
#include <cstdlib>
#include "cell.h"

using namespace std;

class Map
{
private:
    int cellSize;
    int gridWidth;
    int gridHeight;
    int speed;
    vector<vector<Cell>> grid;
    sf::RenderWindow window;
public:
    Map(int cellSize, int speed);
    Map(int cellSize, int gridWidth, int gridHeight, int speed);
    void rule(int x, int y);
    void initializeGrid();
    void renderGrid();
    void start();
    int getCellNeighbor(int x, int y);
};

#endif // MAP_H
