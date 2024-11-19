#include "map.h"
#include "cell.h"
#include <iostream>

using namespace std;

// Constructeur par defaut
Map::Map(int cellSize, int speed)
    : cellSize(cellSize), speed(speed)
{
    // Obtenez la taille de l'écran
    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
    int screenWidth = desktop.width;
    int screenHeight = desktop.height;

    // Calculez le nombre de cellules possibles horizontalement et verticalement
    gridWidth = screenWidth / cellSize;   // Nombre de cellules horizontales
    gridHeight = screenHeight / cellSize; // Nombre de cellules verticales

    // Créez la grille avec les nouvelles dimensions
    grid = vector<vector<Cell>>(gridWidth, vector<Cell>(gridHeight));

    // Créez la fenêtre avec les dimensions calculées
    window.create(sf::VideoMode(screenWidth, screenHeight), "Game of Life");
}

// Constructeur avec paramètres supplémentaires (pour le cas où vous souhaitez spécifier d'autres dimensions)
Map::Map(int cellSize, int gridWidth, int gridHeight, int speed)
    : cellSize(cellSize), gridWidth(gridWidth), gridHeight(gridHeight), speed(speed),
      grid(gridWidth, vector<Cell>(gridHeight)),
      window(sf::VideoMode(gridWidth * cellSize, gridHeight * cellSize), "Game of Life") {}

void Map::initializeGrid()
{
    srand(time(0));
    
    // Déterminer la taille de la zone centrale
    int centerX = gridWidth / 2;
    int centerY = gridHeight / 2;
    int halfSize = gridWidth / 6;  // Taille du carré central

    // Parcourir la grille entière
    for (int x = 0; x < gridWidth; ++x)
    {
        for (int y = 0; y < gridHeight; ++y)
        {
            bool isAlive = false;

            // Vérifier si la cellule est dans la zone centrale
            if (x >= centerX - halfSize && x < centerX + halfSize &&
                y >= centerY - halfSize && y < centerY + halfSize)
            {
                // Appliquer la probabilité de 10% dans la zone centrale
                isAlive = (rand() % 3 != 2);  // 2 chances sur 3 d'être vivante
            }

            // Créer la cellule avec son état (vivante ou morte) et sa position
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
