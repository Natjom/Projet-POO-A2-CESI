#include "grid.hpp"
#include <sstream>
#include <fstream>
#include <limits>

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
            grid[x][y] = Cell(isAlive, x, y);
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
    std::ifstream file("data/" + filename);
    if (!file.is_open())
    {
        std::cerr << "Erreur : impossible d'ouvrir le fichier " << filename << std::endl;
        return;
    }

    int fileRows, fileCols;
    file >> fileRows >> fileCols;

    // Si resizeGrid est vrai, redimensionner la grille aux dimensions du fichier
    if (resizeGrid)
    {
        // Créer une nouvelle grille temporaire de la taille des dimensions du fichier
        vector<vector<Cell>> tempGrid(fileRows, vector<Cell>(fileCols));

        // Lire chaque ligne du fichier et ajouter les données à la nouvelle grille
        std::string line;
        int currentRow = 0;
        while (std::getline(file, line) && currentRow < fileRows)
        {
            std::stringstream lineStream(line);
            int currentCol = 0;

            // Lire les valeurs de la ligne et remplir la grille
            while (lineStream)
            {
                int state;
                lineStream >> state;

                if (currentCol < fileCols)
                {
                    tempGrid[currentRow][currentCol] = Cell(state == 1, currentCol, currentRow); // Remplir les cases avec les données du fichier
                    ++currentCol;
                }
            }

            ++currentRow;
        }

        // Une fois que la nouvelle grille est remplie avec les données, la transférer dans la grille existante
        // Tout le reste de la grille (les dimensions de gridWidth et gridHeight) sera rempli avec des cellules mortes (0).
        gridWidth = fileCols;
        gridHeight = fileRows;
        grid = std::vector<std::vector<Cell>>(gridHeight, std::vector<Cell>(gridWidth)); // Recréer la grille

        // Copier les données de tempGrid dans grid
        for (int i = 0; i < fileRows; ++i)
        {
            for (int j = 0; j < fileCols; ++j)
            {
                grid[i][j] = tempGrid[i][j];
            }
        }

        // Si la grille d'origine est plus grande, remplir les cases restantes avec des cellules mortes (0)
        for (int i = fileRows; i < gridHeight; ++i)
        {
            for (int j = 0; j < gridWidth; ++j)
            {
                grid[i][j] = Cell(false, j, i); // Remplir avec des cellules mortes (0)
            }
        }

    }
    else
    {
        std::cerr << "Erreur : le redimensionnement de la grille est requis pour cette opération." << std::endl;
    }

    file.close();
    std::cout << "Fichier chargé depuis : " << filename << ". Grille redimensionnée : "
              << (resizeGrid ? "Oui" : "Non") << std::endl;
}





Cell &Grid::getCell(int x, int y)
{
    return grid[x][y];
}

void Grid::Export(const std::string &filename)
{
    // Créer le chemin complet du fichier
    std::string filepath = "exports/" + filename;
    std::ofstream file(filepath);

    // Vérifier si le fichier est ouvert
    if (!file.is_open())
    {
        std::cerr << "Erreur : impossible de créer ou d'ouvrir le fichier " << filepath << std::endl;
        return;
    }

    // Écrire les dimensions de la grille
    file << gridHeight << " " << gridWidth << "\n";

    // Parcourir les cellules et écrire leur état dans le fichier
    for (int y = 0; y < gridHeight; ++y)
    {
        for (int x = 0; x < gridWidth; ++x)
        {
            file << (grid[x][y].getState() ? "1" : "0") << " ";
        }
        file << "\n"; // Fin de ligne pour chaque rangée
    }

    // Fermer le fichier
    file.close();
    std::cout << "État exporté vers le fichier : " << filepath << std::endl;
}
