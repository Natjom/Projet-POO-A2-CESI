#include "map.h"
#include "cell.h"
#include <iostream>

using namespace std;

Map::Map(int cellSize, int speed)
    : cellSize(cellSize), speed(speed)
{
    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
    int screenWidth = desktop.width - 10;         // 80% de la largeur de l'écran
    int screenHeight = desktop.height - 200; // 80% de la hauteur moins la barre inférieure
    gridWidth = screenWidth / cellSize;
    gridHeight = screenHeight / cellSize;
    grid = vector<vector<Cell>>(gridWidth, vector<Cell>(gridHeight));
    window.create(sf::VideoMode(screenWidth, screenHeight + 100), "Game of Life");
    window.setPosition(sf::Vector2i(0, 0)); // Place la fenêtre à 50 pixels des bords gauche et supérieur de l'écran
}

void Map::initializeGrid()
{
    srand(time(0));
    int centerX = gridWidth / 2;
    int centerY = gridHeight / 2;
    int halfSize = gridWidth / 6;
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

void Map::handleMouseClick(sf::Vector2i mousePosition)
{
    float buttonWidth = 120;
    float buttonHeight = 40;
    float margin = 20;
    float startX = 10;
    float startY = gridHeight * cellSize + 30; // Position verticale des boutons

    for (int i = 0; i < 3; ++i) // Vérifier les trois boutons
    {
        float buttonX = startX + i * (buttonWidth + margin);
        float buttonY = startY;

        if (mousePosition.x >= buttonX && mousePosition.x <= buttonX + buttonWidth &&
            mousePosition.y >= buttonY && mousePosition.y <= buttonY + buttonHeight)
        {
            cout << "Button " << i + 1 << " clicked!" << endl;
            // Effectuez l'action du bouton ici
        }
    }
}

void Map::renderSidebar()
{
    // Définir la hauteur de la barre inférieure
    float sidebarHeight = 100.0f;
    float sidebarWidth = gridWidth * cellSize;

    // Dessiner la barre inférieure
    sf::RectangleShape sidebar(sf::Vector2f(sidebarWidth, sidebarHeight));
    sidebar.setFillColor(sf::Color(50, 50, 50));   // Couleur gris foncé
    sidebar.setPosition(0, gridHeight * cellSize); // Placée en bas de la grille
    window.draw(sidebar);

    // Dessiner des boutons
    float buttonWidth = 120;
    float buttonHeight = 40;
    float margin = 20;                                                         // Espace entre les boutons
    float startX = 10;                                                         // Position x initiale
    float startY = gridHeight * cellSize + (sidebarHeight - buttonHeight) / 2; // Centrer verticalement

    for (int i = 0; i < 3; ++i) // Trois boutons
    {
        sf::RectangleShape button(sf::Vector2f(buttonWidth, buttonHeight));
        button.setFillColor(sf::Color(100, 100, 100)); // Couleur gris clair
        button.setPosition(startX + i * (buttonWidth + margin), startY);
        window.draw(button);

        // Ajouter du texte (optionnel)
        sf::Font font;
        if (font.loadFromFile("../fonts/arial.ttf")) // Assurez-vous que "arial.ttf" existe
        {
            sf::Text buttonText;
            buttonText.setFont(font);
            buttonText.setString("Button " + std::to_string(i + 1)); // Nom des boutons
            buttonText.setCharacterSize(16);
            buttonText.setFillColor(sf::Color::White);
            buttonText.setPosition(startX + i * (buttonWidth + margin) + 10, startY + 10);
            window.draw(buttonText);
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
    // Applique les règles de la grille
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

    // Dessine la grille
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

    // Dessine la barre latérale avec les boutons
    renderSidebar();

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

        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
        {
            sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
            handleMouseClick(mousePosition);
        }

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
