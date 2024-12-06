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
    speed = 10;    // 100
    pause = false; // true
    devInfo = true;

    int tempCellSize = 8; // 4

    gridWidth = screenWidth / tempCellSize;
    gridHeight = screenHeight / tempCellSize;
    cellSize = std::min(screenWidth / gridWidth, screenHeight / gridHeight);

    window.create(sf::VideoMode(screenWidth, screenHeight + 100), "CESI GAMING");
    window.setPosition(sf::Vector2i(0, 0));
    window.setFramerateLimit(30);

    grid = Grid(gridWidth, gridHeight);

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
    std::thread updateThread([this]()
                             {
        while (true) {
            if (!pause) {
                renderGrid();  // Mise à jour de la grille
                std::this_thread::sleep_for(std::chrono::milliseconds(speed));  // Pause entre les générations
            }
        } });

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

            if (event.type == sf::Event::MouseButtonPressed &&
                (event.mouseButton.button == sf::Mouse::Left || event.mouseButton.button == sf::Mouse::Right))
            {
                sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
                handleMouseClick(mousePosition); // Appelle la gestion du clic de souris
            }
        }

        window.clear(); // Efface la fenêtre pour la nouvelle image

        // Affichage des cellules
        for (int x = 0; x < gridWidth; ++x)
        {
            for (int y = 0; y < gridHeight; ++y)
            {
                sf::RectangleShape cell(sf::Vector2f(cellSize - 1.0f, cellSize - 1.0f));

                // Définir la couleur de la cellule en fonction de son état
                if (grid.getCell(x, y).getObstacle())
                {
                    if (grid.getCell(x, y).getState())
                    {
                        cell.setFillColor(sf::Color::Red); // Cellule vivante
                    }
                    else
                    {
                        cell.setFillColor(sf::Color::Blue); // Cellule morte
                    }
                }
                else
                {
                    if (grid.getCell(x, y).getState())
                    {
                        cell.setFillColor(sf::Color::White); // Cellule vivante
                    }
                    else
                    {
                        cell.setFillColor(sf::Color::Black); // Cellule morte
                    }
                }

                // Positionner la cellule sur la grille
                cell.setPosition(x * cellSize, y * cellSize);
                window.draw(cell); // Dessiner la cellule
            }
        }

        renderSidebar(); // Afficher la barre latérale
        // renderDevInfo();  // Afficher les informations de développement
        window.display(); // Afficher l'ensemble de la scène
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

void Graphic::renderSidebar()
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
    if (font.loadFromFile("fonts/arial.ttf"))
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

    if (font.loadFromFile("fonts/arial.ttf"))
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

    if (font.loadFromFile("fonts/arial.ttf"))
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

    if (font.loadFromFile("fonts/arial.ttf"))
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

    if (font.loadFromFile("fonts/arial.ttf"))
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

    if (font.loadFromFile("fonts/arial.ttf"))
    {
        sf::Text buttonText6("Load Grid", font, 16);
        buttonText6.setFillColor(sf::Color::White);
        buttonText6.setPosition(startX + 5 * (buttonWidth + margin) + 10, startY + 10);
        window.draw(buttonText6);
    }
}

void Graphic::handleMouseClick(sf::Vector2i mousePosition)
{
    if (mousePosition.y < gridHeight * cellSize)
    {
        int cellX = mousePosition.x / cellSize;
        int cellY = mousePosition.y / cellSize;
        if (cellX >= 0 && cellX < gridWidth && cellY >= 0 && cellY < gridHeight)
        {
            // Si le clic est à gauche, inverser l'état de la cellule
            if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
            {
                grid.getCell(cellX, cellY).setState(!grid.getCell(cellX, cellY).getState(), true);
                if (devInfo)
                    cout << "Cellule (" << cellX << ", " << cellY << ") inversée (clic gauche)." << endl;
            }
            // Si le clic est à droite, définir la cellule comme un obstacle
            else if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
            {
                grid.getCell(cellX, cellY).setObstacle(!grid.getCell(cellX, cellY).getObstacle()); // Met la cellule comme un obstacle
                if (devInfo)
                    cout << "Cellule (" << cellX << ", " << cellY << ") obstacle créé (clic droit)." << endl;
            }
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
        if (devInfo)
            cout << "Bouton Pause/Play cliqué !" << endl;
    }
    if (mousePosition.x >= startX + (buttonWidth + margin) && mousePosition.x <= startX + (buttonWidth + margin) + buttonWidth &&
        mousePosition.y >= startY && mousePosition.y <= startY + buttonHeight)
    {
        grid.initializeGrid();
        if (devInfo)
            cout << "Bouton Réinitialiser cliqué !" << endl;
    }
    if (mousePosition.x >= startX + 2 * (buttonWidth + margin) && mousePosition.x <= startX + 2 * (buttonWidth + margin) + buttonWidth &&
        mousePosition.y >= startY && mousePosition.y <= startY + buttonHeight)
    {
        window.close();
        if (devInfo)
            cout << "Bouton Quitter cliqué !" << endl;
    }
    if (mousePosition.x >= startX + 3 * (buttonWidth + margin) && mousePosition.x <= startX + 3 * (buttonWidth + margin) + buttonWidth &&
        mousePosition.y >= startY && mousePosition.y <= startY + buttonHeight)
    {
        speed = std::min(speed * 10, 2000);
        sf::sleep(sf::milliseconds(100));
        if (devInfo)
            cout << "Vitesse +, nouvelle vitesse: " << speed << "ms" << endl;
    }
    if (mousePosition.x >= startX + 4 * (buttonWidth + margin) && mousePosition.x <= startX + 4 * (buttonWidth + margin) + buttonWidth &&
        mousePosition.y >= startY && mousePosition.y <= startY + buttonHeight)
    {
        speed = std::max(speed / 10, 1);
        sf::sleep(sf::milliseconds(100));
        if (devInfo)
            cout << "Vitesse -, nouvelle vitesse: " << speed << "ms" << endl;
    }

    if (mousePosition.x >= startX + 5 * (buttonWidth + margin) && mousePosition.x <= startX + 5 * (buttonWidth + margin) + buttonWidth &&
        mousePosition.y >= startY && mousePosition.y <= startY + buttonHeight)
    {
        std::string filename;
        cout << "Entrez le chemin du fichier à charger : ";
        cin >> filename; // Vous pouvez remplacer ceci par un dialogue graphique.
        grid.loadGridFromFile(filename, false);
        cout << "Grille chargée depuis : " << filename << endl;
    }
}
