#include "grid.hpp"
#include <sstream>
#include <fstream>
#include <limits>
#include <iostream>
#include <vector>

using namespace std;

Grid::Grid() : gridWidth(0), gridHeight(0), grid(0, vector<Cell>(0))
{
}

Grid::Grid(int gridWidth, int gridHeight)
    : gridWidth(gridWidth), gridHeight(gridHeight), grid(gridWidth, vector<Cell>(gridHeight))
{
}

void Grid::initializeGrid()
{
    srand(static_cast<unsigned int>(time(0)));  // Utilisation du bon type pour srand
    for (int x = 0; x < gridWidth; ++x)
    {
        for (int y = 0; y < gridHeight; ++y)
        {
            bool isAlive = rand() % 2;  // Le binaire est plus simple ici
            grid[x][y] = Cell(isAlive, false, x, y);
        }
    }
}

int Grid::getCellNeighbor(int x, int y) const
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

void Grid::loadGridFromFile(const string &filename, bool resizeGrid)
{
    string filepath = "./data/" + filename;
    ifstream file(filepath);

    if (!file.is_open())
    {
        cerr << "File cannot be opened: " << filepath << endl;
        return;
    }

    int motifRows, motifCols;
    file >> motifRows >> motifCols;
    file.ignore(numeric_limits<streamsize>::max(), '\n');

    vector<vector<Cell>> newGrid(motifRows, vector<Cell>(motifCols));
    string line;
    int y = 0;
    while (getline(file, line) && y < motifRows)
    {
        istringstream lineStream(line);
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
                int state = newGrid[x][y].getState();
                if (startX + x >= 0 && startX + x < gridWidth && startY + y >= 0 && startY + y < gridHeight)
                {
                    grid[startX + x][startY + y] = Cell(state == 1, false, startX + x, startY + y);
                }
            }
        }
    }

    file.close();
    cout << "File loaded from: " << filepath << endl;
}

Cell &Grid::getCell(int x, int y)
{
    return grid[x][y];
}

void Grid::Export(const string &filename) const
{
    ofstream file(filename);

    if (!file.is_open())
    {
        cerr << "File cannot be created: " << filename << endl;
        return;
    }

    file << gridHeight << " " << gridWidth << "\n";

    for (int y = 0; y < gridHeight; ++y)
    {
        for (int x = 0; x < gridWidth; ++x)
        {
            file << (grid[y][x].getState() ? "1" : "0") << " ";
        }
        file << "\n";
    }

    file.close();
}
