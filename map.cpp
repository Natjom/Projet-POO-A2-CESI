#include "map.h"
#include "cell.h"
#include <iostream>

using namespace std;

// Constructeur par defaut
Map::Map()
    : cellSize(10), gridWidth(80), gridHeight(80), speed(1000),
      grid(80, vector<Cell>(80)),
      window(sf::VideoMode(80 * 10, 80 * 10), "Game of Life") {}

// Constructeur par parametres
Map::Map(int cellSize, int gridWidth, int gridHeight, int speed)
    : cellSize(cellSize), gridWidth(gridWidth), gridHeight(gridHeight), speed(speed),
      grid(gridWidth, vector<Cell>(gridHeight)),
      window(sf::VideoMode(gridWidth * cellSize, gridHeight * cellSize), "Game of Life") {}

void Map::initializeGrid()
{
    srand(time(0));
    int centerX = gridWidth / 2;
    int centerY = gridHeight / 2;
    int halfSize = gridWidth / 4;
    for (int x = 0; x < gridWidth; ++x)
    {
        for (int y = 0; y < gridHeight; ++y)
        {
            bool isAlive = false;
            if (x >= centerX - halfSize && x < centerX + halfSize &&
                y >= centerY - halfSize && y < centerY + halfSize)
            {
                isAlive = (rand() % 3 != 2); 
            }
            grid[x][y] = Cell(isAlive, x, y);
        }
    }
}


void Map::rule(int x, int y)
{
    int neighborCount = getCellNeighbor(x, y);
    if (grid[x][y].getState())
    {
        if (neighborCount != 2 && neighborCount != 3)
        {
            grid[x][y].setState(false);
        }
        else
        {
            grid[x][y].setState(true);
        }
    }
    else
    {
        if (neighborCount == 3)
        {
            grid[x][y].setState(true); 
        }
        else
        {
            grid[x][y].setState(false); 
        }
    }
}

void Map::renderGrid()
{
    for (int x = 0; x < gridWidth; ++x)
    {
        for (int y = 0; y < gridHeight; ++y)
        {
            rule(x, y);
        }
    }
    for (int x = 0; x < gridWidth; ++x)
    {
        for (int y = 0; y < gridHeight; ++y)
        {
            grid[x][y].update();
        }
    }
    window.clear();
    sf::RectangleShape cell(sf::Vector2f(cellSize - 1.0f, cellSize - 1.0f));

    for (int x = 0; x < gridWidth; ++x)
    {
        for (int y = 0; y < gridHeight; ++y)
        {
            if (grid[x][y].getState())
            {
                cell.setFillColor(sf::Color::White);
            }
            else
            {
                cell.setFillColor(sf::Color::Black);
            }

            cell.setPosition(x * cellSize, y * cellSize);
            window.draw(cell);
        }
    }
    window.display();
}

void Map::start()
{
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        renderGrid();

        sf::sleep(sf::milliseconds(speed));
    }
}

int Map::getCellNeighbor(int x, int y)
{
    int count = 0;
    for (int dx = -1; dx <= 1; ++dx)
    {
        for (int dy = -1; dy <= 1; ++dy)
        {
            if (dx == 0 && dy == 0)
            {
                continue;  
            }
            int nx = (x + dx + gridWidth) % gridWidth;  
            int ny = (y + dy + gridHeight) % gridHeight; 
            if (grid[nx][ny].getState())
            {
                ++count;
            }
        }
    }
    return count;
}
