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
    vector<vector<Cell>> grid;
    sf::RenderWindow window;
public:
    //Constructeurs
    Map();
    Map(int cellSize, int gridWidth, int gridHeight);


    //Methodes

    void initializeGrid();
    void renderGrid();
    void start();


    int getCellNeighbor(int x, int y);
};

#endif // MAP_H
