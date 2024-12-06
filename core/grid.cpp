#include "grid.hpp"
#include <sstream>
#include <fstream>
#include <limits>
#include <iostream>
#include <vector>

using namespace std;

Grid::Grid()
{
    grid = vector<vector<Cell>>(0, vector<Cell>(0));
}

Grid::Grid(int gridWidth, int gridHeight)
{
    this->gridWidth = gridWidth;
    this->gridHeight = gridHeight;
    grid = vector<vector<Cell>>(gridWidth, vector<Cell>(gridHeight));
}

void Grid::initializeGrid()
{
    srand(time(0));
    for (int x = 0; x < gridWidth; ++x)
    {
        for (int y = 0; y < gridHeight; ++y)
        {
            bool isAlive = false;
            isAlive = (rand() % 2 != 1);
            grid[x][y] = Cell(isAlive, false, x, y);
        }
    }
}

int Grid::getCellNeighbor(int x, int y)
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

void Grid::rule(int x, int y)
{
    if (grid[x][y].getObstacle())
    {
        return;
    }

    int neighborCount = getCellNeighbor(x, y);
    if (grid[x][y].getState())
    {
        if (neighborCount != 2 && neighborCount != 3)
        {
            grid[x][y].setState(false, false);
        }
        else
        {
            grid[x][y].setState(true, false);
        }
    }
    else
    {
        if (neighborCount == 3)
        {
            grid[x][y].setState(true, false);
        }
        else
        {
            grid[x][y].setState(false, false);
        }
    }
}

void Grid::update()
{
    for (int x = 0; x < gridWidth; ++x)
    {
        for (int y = 0; y < gridHeight; ++y)
        {
            grid[x][y].update();
        }
    }
}

void Grid::loadGridFromFile(const std::string &filename, bool resizeGrid)
{
    std::string filepath = "./data/" + filename;
    std::ifstream file(filepath);

    if (!file.is_open())
    {
        std::cout << "file not openable" << std::endl;
        return;
    }

    int motifRows, motifCols;
    file >> motifRows >> motifCols;
    file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    vector<vector<Cell>> newGrid(motifRows, vector<Cell>(motifCols));
    std::string line;
    int y = 0;
    while (getline(file, line) && y < motifRows)
    {
        std::istringstream lineStream(line);
        int state, x = 0;

        while (lineStream >> state && x < motifCols)
        {
            newGrid[y][x] = Cell(state == 1, false, x, y);
            ++x;
        }
        ++y;
    }

    if (resizeGrid)
    {

        // Met à jour la grille
        grid.swap(newGrid);
        gridWidth = motifCols;
        gridHeight = motifRows;
    }

    else
    {
        for (int x = 0; x < gridWidth; ++x)
        {
            for (int y = 0; y < gridHeight; ++y)
            {
                grid[x][y] = Cell(false, false, x, y);
            }
        }
        int startX = (gridWidth - motifCols) / 2;
        int startY = (gridHeight - motifRows) / 2;
        for (int y = 0; y < motifRows; ++y)
        {
            for (int x = 0; x < motifCols; ++x)
            {
                int state;
                state = newGrid[x][y].getState();
                if (startX + x >= 0 && startX + x < gridWidth && startY + y >= 0 && startY + y < gridHeight)
                {
                    grid[startX + x][startY + y] = Cell(state == 1, false, startX + x, startY + y);
                }
            }
        }
        file.close();
        std::cout << "Fichier chargé depuis : " << filepath << " et placé au centre de la grille." << std::endl;
    }

    file.close();
}

Cell &Grid::getCell(int x, int y)
{
    return grid[x][y];
}

void Grid::Export(const std::string &filename)
{
    // Créer le chemin complet du fichier
    std::ofstream file(filename);

    // Vérifier si le fichier est ouvert
    if (!file.is_open())
    {
        return;
    }

    // Écrire les dimensions de la grille
    file << gridHeight << " " << gridWidth << "\n";

    // Parcourir les cellules et écrire leur état dans le fichier
    for (int y = 0; y < gridHeight; ++y)
    {
        for (int x = 0; x < gridWidth; ++x)
        {
            file << (grid[y][x].getState() ? "1" : "0") << " ";
        }
        file << "\n"; // Fin de ligne pour chaque rangée
    }

    // Fermer le fichier
    file.close();
}
