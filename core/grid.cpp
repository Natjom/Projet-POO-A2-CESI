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
    srand(static_cast<unsigned int>(time(0))); // Utilisation du bon type pour srand
    for (int x = 0; x < gridWidth; ++x)
    {
        for (int y = 0; y < gridHeight; ++y)
        {
            bool isAlive = rand() % 2; // Le binaire est plus simple ici
            grid[x][y] = Cell(isAlive, false, x, y);
        }
    }
}

int Grid::getCellNeighbor(int x, int y) const
{
    int count = 0; // Variable pour compter le nombre de voisins "vivants" autour de la cellule (x, y)

    // Parcours de tous les voisins possibles dans une zone 3x3 autour de la cellule
    for (int dx = -1; dx <= 1; ++dx)
    {
        for (int dy = -1; dy <= 1; ++dy)
        {
            if (dx == 0 && dy == 0) // Ignore la cellule elle-même
            {
                continue; // Passe à l'itération suivante
            }

            // Calcul des coordonnées du voisin en tenant compte du wrapping (les bords sont reliés)
            int nx = (x + dx + gridWidth) % gridWidth;   // Utilise le modulo pour rendre les indices "cycliques"
            int ny = (y + dy + gridHeight) % gridHeight; // Idem pour la hauteur de la grille

            // Si la cellule voisine est "vivante", on incrémente le compteur
            if (grid[nx][ny].getState()) // Vérifie si la cellule voisine est dans l'état "vivant"
            {
                ++count; // Compte un voisin "vivant"
            }
        }
    }

    return count; // Retourne le nombre total de voisins vivants
}

void Grid::rule(int x, int y)
{
    // Si la cellule est un obstacle, on ne fait rien et on retourne immédiatement
    if (grid[x][y].getObstacle())
    {
        return; // Les obstacles ne changent pas d'état
    }

    // Récupère le nombre de voisins vivants pour la cellule (x, y)
    int neighborCount = getCellNeighbor(x, y);

    // Si la cellule est vivante
    if (grid[x][y].getState())
    {
        // Application des règles du jeu de la vie (Conway's Game of Life)
        // La cellule vivante reste vivante si elle a 2 ou 3 voisins vivants
        if (neighborCount != 2 && neighborCount != 3)
        {
            grid[x][y].setState(false, false); // Si elle n'a pas 2 ou 3 voisins, elle meurt
        }
        else
        {
            grid[x][y].setState(true, false); // Si elle a 2 ou 3 voisins, elle reste vivante
        }
    }
    // Si la cellule est morte
    else
    {
        // La cellule morte devient vivante si elle a exactement 3 voisins vivants
        if (neighborCount == 3)
        {
            grid[x][y].setState(true, false); // Elle naît
        }
    }
}

void Grid::update()
{
    // Parcours de chaque cellule de la grille
    for (int x = 0; x < gridWidth; ++x)
    {
        for (int y = 0; y < gridHeight; ++y)
        {
            // Met à jour l'état de la cellule à la position (x, y)
            grid[x][y].update();
        }
    }
}

void Grid::loadGridFromFile(const string &filename, bool resizeGrid)
{
    // Crée le chemin complet du fichier à partir du nom du fichier
    string filepath = "./data/" + filename;

    // Ouvre le fichier
    ifstream file(filepath);

    // Si le fichier ne peut pas être ouvert, affiche un message d'erreur
    if (!file.is_open())
    {
        cerr << "Impossible d'ouvrir le fichier : " << filepath << endl;
        return;
    }

    // Lecture des dimensions du motif à partir du fichier
    int motifRows, motifCols;
    file >> motifRows >> motifCols;

    // Vérifie que les dimensions lues sont valides
    if (motifRows <= 0 || motifCols <= 0)
    {
        cerr << "Dimensions invalides dans le fichier : " << filepath << endl;
        file.close();
        return;
    }

    file.ignore(numeric_limits<streamsize>::max(), '\n'); // Ignore le reste de la ligne après les dimensions

    // Crée une nouvelle grille avec les dimensions du motif
    vector<vector<Cell>> newGrid(motifRows, vector<Cell>(motifCols));

    // Lecture du motif ligne par ligne
    string line;
    int y = 0;
    while (getline(file, line) && y < motifRows)
    {
        istringstream lineStream(line);
        int state, x = 0;

        // Lecture de l'état des cellules pour chaque ligne
        while (lineStream >> state && x < motifCols)
        {
            newGrid[y][x] = Cell(state == 1, false, x, y); // État de la cellule
            ++x;
        }
        ++y;
    }

    // Si on doit redimensionner la grille
    if (resizeGrid)
    {
        grid.swap(newGrid);     // Remplace l'ancienne grille par la nouvelle
        gridWidth = motifCols;  // Met à jour la largeur
        gridHeight = motifRows; // Met à jour la hauteur
    }
    else
    {
        // Vérifie que la grille existante a des dimensions valides
        if (gridWidth <= 0 || gridHeight <= 0)
        {
            cerr << "Grille actuelle invalide pour placer le motif." << endl;
            file.close();
            return;
        }

        // Réinitialise la grille existante (toutes les cellules à mortes)
        for (int y = 0; y < gridHeight; ++y)
        {
            for (int x = 0; x < gridWidth; ++x)
            {
                grid[x][y] = Cell(false, false, x, y); // Met toutes les cellules à mortes
            }
        }

        // Calcule les coordonnées de départ pour centrer le motif
        int startX = (gridWidth - motifCols) / 2;
        int startY = (gridHeight - motifRows) / 2;

        // Place le motif au centre de la grille
        for (int y = 0; y < motifRows; ++y)
        {
            for (int x = 0; x < motifCols; ++x)
            {
                int destX = startX + x;
                int destY = startY + y;

                // Vérifie que les coordonnées calculées sont valides avant d'insérer
                if (destX >= 0 && destX < gridWidth && destY >= 0 && destY < gridHeight)
                {
                    grid[destX][destY] = newGrid[x][y];
                }
            }
        }
    }
    // Ferme le fichier après lecture
    file.close();
    cout << "Fichier chargé depuis : " << filepath << endl;
}

Cell &Grid::getCell(int x, int y)
{
    return grid[x][y];
}

void Grid::Export(const string &filename) const
{
    // Ouvre un fichier en écriture pour sauvegarder l'état de la grille
    ofstream file(filename);

    // Si le fichier ne peut pas être ouvert, affiche un message d'erreur
    if (!file.is_open())
    {
        cerr << "File cannot be created: " << filename << endl;
        return;
    }

    // Écrit les dimensions de la grille (hauteur et largeur) dans le fichier
    file << gridHeight << " " << gridWidth << "\n";

    // Parcourt toutes les cellules de la grille pour écrire leur état
    for (int y = 0; y < gridHeight; ++y)
    {
        for (int x = 0; x < gridWidth; ++x)
        {
            // Écrit "1" si la cellule est vivante, sinon "0"
            file << (grid[y][x].getState() ? "1" : "0") << " ";
        }
        // Ajoute une nouvelle ligne après chaque ligne de la grille
        file << "\n";
    }

    // Ferme le fichier une fois l'écriture terminée
    file.close();
}
