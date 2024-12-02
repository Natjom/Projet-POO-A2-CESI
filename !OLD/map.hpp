#ifndef MAP_H
#define MAP_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <ctime>
#include <cstdlib>
#include "cell.hpp"

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
    bool pause;
    bool devInfo;
public:
    Map();
    void rule(int x, int y);
    void initializeGrid();
    void renderGrid();
    void start();
    void handleMouseClick(sf::Vector2i mousePosition);
    void renderSidebar();
    int getCellNeighbor(int x, int y);
    void updateSection(int startX, int endX, int startY, int endY);
    void renderDevInfo();
    void loadGridFromFile(const std::string &filename);
};

#endif // MAP_H