#include "map.h"
#include <iostream>

using namespace std;

// Constructeur par defaut
Map::Map()
    : cellSize(10), gridWidth(80), gridHeight(80), grid(gridWidth, vector<int>(gridHeight, 0)), window(sf::VideoMode(gridWidth * cellSize, gridHeight * cellSize), "Game of Life")
{

}

// Constructeur par parametres
Map::Map(int cellSize, int gridWidth, int gridHeight)
    : cellSize(cellSize), gridWidth(gridWidth), gridHeight(gridHeight), grid(gridWidth, vector<int>(gridHeight, 0)), window(sf::VideoMode(gridWidth * cellSize, gridHeight * cellSize), "Game of Life")
{
}


void Map::initializeGrid() {;
    srand(time(0));
    for (int x = 0; x < gridWidth; ++x) {
        for (int y = 0; y < gridHeight; ++y) {
            grid[x][y] = std::rand() % 2;
        }
    }
};

void Map::renderGrid(sf::RenderWindow &window) {
    int x, y;
    
    window.clear();
    sf::RectangleShape cell(sf::Vector2f(cellSize - 1.0f, cellSize - 1.0f));
    for (x = 0; x < gridWidth; ++x) {
        for (y = 0; y < gridHeight; ++y) {
            if (grid[x][y] == 1) {
                cell.setPosition(x * cellSize, y * cellSize);
                window.draw(cell);
            }
        }
    }
    window.display();
};

void Map::start() 
{
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        renderGrid(window);

        sf::sleep(sf::milliseconds(100));
    }
};