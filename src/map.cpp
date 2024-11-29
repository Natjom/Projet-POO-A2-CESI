#include "map.hpp"
#include "cell.hpp"
#include <iostream>

#include <thread>
#include <atomic>
#include <sstream>
#include <fstream>
#include <chrono>
#include <vector>

using namespace std;

std::atomic<bool> isRunning(true);

Map::Map()
{
    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
    int screenWidth = desktop.width - 10;
    int screenHeight = desktop.height - 200;
    speed = 100;
    pause = false;
    devInfo = true;

    int tempCellSize = 8; // 4 = best

    gridWidth = screenWidth / tempCellSize;
    gridHeight = screenHeight / tempCellSize;
    cellSize = std::min(screenWidth / gridWidth, screenHeight / gridHeight);

    grid = vector<vector<Cell>>(gridWidth, vector<Cell>(gridHeight));

    window.create(sf::VideoMode(screenWidth, screenHeight + 100), "Game of Life");
    window.setPosition(sf::Vector2i(0, 0));

    initializeGrid();
}

void Map::initializeGrid()
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

void Map::handleMouseClick(sf::Vector2i mousePosition)
{

    if (mousePosition.y < gridHeight * cellSize)
    {
        int cellX = mousePosition.x / cellSize;
        int cellY = mousePosition.y / cellSize;

        if (cellX >= 0 && cellX < gridWidth && cellY >= 0 && cellY < gridHeight)
        {
            grid[cellX][cellY].setState(!grid[cellX][cellY].getState());
            cout << "Cellule (" << cellX << ", " << cellY << ") inversée." << endl;
        }
    }

    float buttonWidth = 120;
    float buttonHeight = 40;
    float margin = 20;
    float startX = 10;
    float startY = gridHeight * cellSize + 30;

    if (mousePosition.x >= startX && mousePosition.x <= startX + buttonWidth &&
        mousePosition.y >= startY && mousePosition.y <= startY + buttonHeight)
    {
        pause = !pause;
        sf::sleep(sf::milliseconds(100));
        cout << "Bouton Pause/Play cliqué !" << endl;
    }
    if (mousePosition.x >= startX + (buttonWidth + margin) && mousePosition.x <= startX + (buttonWidth + margin) + buttonWidth &&
        mousePosition.y >= startY && mousePosition.y <= startY + buttonHeight)
    {
        initializeGrid();
        cout << "Bouton Réinitialiser cliqué !" << endl;
    }
    if (mousePosition.x >= startX + 2 * (buttonWidth + margin) && mousePosition.x <= startX + 2 * (buttonWidth + margin) + buttonWidth &&
        mousePosition.y >= startY && mousePosition.y <= startY + buttonHeight)
    {
        window.close();
        cout << "Bouton Quitter cliqué !" << endl;
    }
    if (mousePosition.x >= startX + 3 * (buttonWidth + margin) && mousePosition.x <= startX + 3 * (buttonWidth + margin) + buttonWidth &&
        mousePosition.y >= startY && mousePosition.y <= startY + buttonHeight)
    {
        speed = std::min(speed * 10, 2000);
        sf::sleep(sf::milliseconds(100));
        cout << "Vitesse +, nouvelle vitesse: " << speed << "ms" << endl;
    }
    if (mousePosition.x >= startX + 4 * (buttonWidth + margin) && mousePosition.x <= startX + 4 * (buttonWidth + margin) + buttonWidth &&
        mousePosition.y >= startY && mousePosition.y <= startY + buttonHeight)
    {
        speed = std::max(speed / 10, 1);
        sf::sleep(sf::milliseconds(100));
        cout << "Vitesse -, nouvelle vitesse: " << speed << "ms" << endl;
    }

    if (mousePosition.x >= startX + 5 * (buttonWidth + margin) && mousePosition.x <= startX + 5 * (buttonWidth + margin) + buttonWidth &&
        mousePosition.y >= startY && mousePosition.y <= startY + buttonHeight)
    {
        std::string filename;
        cout << "Entrez le chemin du fichier à charger : ";
        cin >> filename; // Vous pouvez remplacer ceci par un dialogue graphique.
        loadGridFromFile(filename);
        cout << "Grille chargée depuis : " << filename << endl;
    }
}

void Map::renderSidebar()
{
    float sidebarHeight = 100.0f;
    float sidebarWidth = gridWidth * cellSize;

    sf::RectangleShape sidebar(sf::Vector2f(sidebarWidth, sidebarHeight));
    sidebar.setFillColor(sf::Color(50, 50, 50));
    sidebar.setPosition(0, gridHeight * cellSize);
    window.draw(sidebar);

    float buttonWidth = 120;
    float buttonHeight = 40;
    float margin = 20;
    float startX = 10;
    float startY = gridHeight * cellSize + 30;

    sf::RectangleShape button1(sf::Vector2f(buttonWidth, buttonHeight));
    button1.setFillColor(sf::Color(100, 100, 100));
    button1.setPosition(startX, startY);
    window.draw(button1);

    sf::Font font;
    if (font.loadFromFile("../fonts/arial.ttf"))
    {
        sf::Text buttonText1("Pause/Play", font, 16);
        buttonText1.setFillColor(sf::Color::White);
        buttonText1.setPosition(startX + 10, startY + 10);
        window.draw(buttonText1);
    }

    sf::RectangleShape button2(sf::Vector2f(buttonWidth, buttonHeight));
    button2.setFillColor(sf::Color(100, 100, 100));
    button2.setPosition(startX + (buttonWidth + margin), startY);
    window.draw(button2);

    if (font.loadFromFile("../fonts/arial.ttf"))
    {
        sf::Text buttonText2("Reset Grid", font, 16);
        buttonText2.setFillColor(sf::Color::White);
        buttonText2.setPosition(startX + (buttonWidth + margin) + 10, startY + 10);
        window.draw(buttonText2);
    }

    sf::RectangleShape button3(sf::Vector2f(buttonWidth, buttonHeight));
    button3.setFillColor(sf::Color(100, 100, 100));
    button3.setPosition(startX + 2 * (buttonWidth + margin), startY);
    window.draw(button3);

    if (font.loadFromFile("../fonts/arial.ttf"))
    {
        sf::Text buttonText3("Quit", font, 16);
        buttonText3.setFillColor(sf::Color::White);
        buttonText3.setPosition(startX + 2 * (buttonWidth + margin) + 10, startY + 10);
        window.draw(buttonText3);
    }

    sf::RectangleShape button4(sf::Vector2f(buttonWidth, buttonHeight));
    button4.setFillColor(sf::Color(100, 100, 100));
    button4.setPosition(startX + 3 * (buttonWidth + margin), startY);
    window.draw(button4);

    if (font.loadFromFile("../fonts/arial.ttf"))
    {
        sf::Text buttonText4("Speed-", font, 16);
        buttonText4.setFillColor(sf::Color::White);
        buttonText4.setPosition(startX + 3 * (buttonWidth + margin) + 10, startY + 10);
        window.draw(buttonText4);
    }

    sf::RectangleShape button5(sf::Vector2f(buttonWidth, buttonHeight));
    button5.setFillColor(sf::Color(100, 100, 100));
    button5.setPosition(startX + 4 * (buttonWidth + margin), startY);
    window.draw(button5);

    if (font.loadFromFile("../fonts/arial.ttf"))
    {
        sf::Text buttonText5("Speed+", font, 16);
        buttonText5.setFillColor(sf::Color::White);
        buttonText5.setPosition(startX + 4 * (buttonWidth + margin) + 10, startY + 10);
        window.draw(buttonText5);
    }

    sf::RectangleShape button6(sf::Vector2f(buttonWidth, buttonHeight));
    button6.setFillColor(sf::Color(100, 100, 100));
    button6.setPosition(startX + 5 * (buttonWidth + margin), startY);
    window.draw(button6);

    if (font.loadFromFile("../fonts/arial.ttf"))
    {
        sf::Text buttonText6("Load Grid", font, 16);
        buttonText6.setFillColor(sf::Color::White);
        buttonText6.setPosition(startX + 5 * (buttonWidth + margin) + 10, startY + 10);
        window.draw(buttonText6);
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
    int numThreads = std::thread::hardware_concurrency();
    std::vector<std::thread> threads;

    int sectionWidth = gridWidth / numThreads;

    for (int i = 0; i < numThreads; ++i)
    {
        int startX = i * sectionWidth;
        int endX = (i == numThreads - 1) ? gridWidth : (i + 1) * sectionWidth;
        threads.emplace_back(&Map::updateSection, this, startX, endX, 0, gridHeight);
    }
    for (auto &thread : threads)
    {
        thread.join();
    }
    for (int x = 0; x < gridWidth; ++x)
    {
        for (int y = 0; y < gridHeight; ++y)
        {
            grid[x][y].update();
        }
    }
}

void Map::start()
{
    sf::Clock clock;
    sf::Time elapsed;
    std::thread updateThread([this]()
                             {
        while (isRunning) {
            if (!pause) {
                renderGrid();
                std::this_thread::sleep_for(std::chrono::milliseconds(speed));
            }
        } });
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                isRunning = false;
                updateThread.join();
                window.close();
            }

            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
            {
                sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
                handleMouseClick(mousePosition);
            }
        }
        window.clear();
        for (int x = 0; x < gridWidth; ++x)
        {
            for (int y = 0; y < gridHeight; ++y)
            {
                sf::RectangleShape cell(sf::Vector2f(cellSize - 1.0f, cellSize - 1.0f));
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
        renderSidebar();
        renderDevInfo();
        window.display();
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

void Map::renderDevInfo()
{
    if (!devInfo)
        return;
    sf::Font font;
    if (!font.loadFromFile("../fonts/arial.ttf"))
        return;
    sf::Text devText;
    devText.setFont(font);
    devText.setString("Dev Info:\nSpeed: " + std::to_string(speed) + " ms");
    devText.setCharacterSize(16);
    devText.setFillColor(sf::Color::Green);
    devText.setPosition(10, 10);
    window.draw(devText);
}

void Map::updateSection(int startX, int endX, int startY, int endY)
{
    for (int x = startX; x < endX; ++x)
    {
        for (int y = startY; y < endY; ++y)
        {
            rule(x, y);
        }
    }
}

void Map::loadGridFromFile(const std::string &filename)
{
    std::string filepath = "../data/" + filename;
    std::ifstream file(filepath);
    if (!file.is_open())
    {
        std::cerr << "Erreur : impossible d'ouvrir le fichier " << filepath << std::endl;
        return;
    }
    int motifRows, motifCols;
    file >> motifRows >> motifCols;
    for (int x = 0; x < gridWidth; ++x)
    {
        for (int y = 0; y < gridHeight; ++y)
        {
            grid[x][y] = Cell(false, x, y);
        }
    }
    int startX = (gridWidth - motifCols) / 2;
    int startY = (gridHeight - motifRows) / 2;
    for (int y = 0; y < motifRows; ++y)
    {
        for (int x = 0; x < motifCols; ++x)
        {
            int state;
            file >> state;
            if (startX + x >= 0 && startX + x < gridWidth && startY + y >= 0 && startY + y < gridHeight)
            {
                grid[startX + x][startY + y] = Cell(state == 1, startX + x, startY + y);
            }
        }
    }
    file.close();
    std::cout << "Fichier chargé depuis : " << filepath << " et placé au centre de la grille." << std::endl;
}
