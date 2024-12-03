#include "graphic.hpp"

#include <iostream>
#include <thread>
#include <atomic>
#include <sstream>
#include <fstream>
#include <chrono>
#include <vector>

using namespace std;

Graphic::Graphic()
{
    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
    int screenWidth = desktop.width - 10;
    int screenHeight = desktop.height - 200;
    speed = 1000; // 100
    pause = false; // true
    devInfo = true;

    int tempCellSize = 4; // 4

    gridWidth = screenWidth / tempCellSize;
    gridHeight = screenHeight / tempCellSize;
    cellSize = std::min(screenWidth / gridWidth, screenHeight / gridHeight);

    window.create(sf::VideoMode(screenWidth, screenHeight + 100), "CESI GAMING");
    window.setPosition(sf::Vector2i(0, 0));

    grid = Grid(screenWidth, screenHeight);
    grid.initializeGrid();
}

Graphic::~Graphic()
{
}

void Graphic::renderGrid()
{
    int numThreads = thread::hardware_concurrency();
    vector<thread> threads;

    int sectionWidth = gridWidth / numThreads;

    for (int i = 0; i < numThreads; ++i)
    {
        int startX = i * sectionWidth;
        int endX = (i == numThreads - 1) ? gridWidth : (i + 1) * sectionWidth;
        threads.emplace_back(&Graphic::updateSection, this, startX, endX, 0, gridHeight);
    }
    for (auto &thread : threads)
    {
        thread.join();
    }
    for (int x = 0; x < gridWidth; ++x)
    {
        for (int y = 0; y < gridHeight; ++y)
        {
            grid.getCell(x, y).update();
        }
    }
}

void Graphic::updateSection(int startX, int endX, int startY, int endY)
{
    for (int x = startX; x < endX; ++x)
    {
        for (int y = startY; y < endY; ++y)
        {
            grid.rule(x, y);
        }
    }
}

void Graphic::start()
{
    sf::Clock clock;
    sf::Time elapsed;
    std::thread updateThread([this](){
        while (true) {
            if (!pause) {
                renderGrid();  // Mise à jour de la grille
                std::this_thread::sleep_for(std::chrono::milliseconds(speed));  // Pause entre les générations
            }
        }
    });

    // Boucle principale pour gérer les événements et redessiner la fenêtre
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                break;
                updateThread.join();
                window.close();
            }

            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
            {
                // sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
                // handleMouseClick(mousePosition);  // Appelle la gestion du clic de souris
            }
        }

        window.clear();  // Efface la fenêtre pour la nouvelle image

        // Affichage des cellules
        for (int x = 0; x < gridWidth; ++x)
        {
            for (int y = 0; y < gridHeight; ++y)
            {
                sf::RectangleShape cell(sf::Vector2f(cellSize - 1.0f, cellSize - 1.0f));

                // Définir la couleur de la cellule en fonction de son état
                if (grid.getCell(x, y).getState())
                {
                    cell.setFillColor(sf::Color::White);  // Cellule vivante
                }
                else
                {
                    cell.setFillColor(sf::Color::Black);  // Cellule morte
                }

                // Positionner la cellule sur la grille
                cell.setPosition(x * cellSize, y * cellSize);
                window.draw(cell);  // Dessiner la cellule
            }
        }

        // renderSidebar();  // Afficher la barre latérale
        // renderDevInfo();  // Afficher les informations de développement
        window.display();  // Afficher l'ensemble de la scène
    }
}

void Graphic::renderDevInfo()
{
    if (!devInfo)
        return;
    sf::Font font;
    if (!font.loadFromFile("../fonts/arial.ttf"))
        return;
    sf::Text devText;
    devText.setFont(font);
    devText.setString("Dev Info:\nSpeed: " + to_string(speed) + " ms");
    devText.setCharacterSize(16);
    devText.setFillColor(sf::Color::Green);
    devText.setPosition(10, 10);
    window.draw(devText);
}

