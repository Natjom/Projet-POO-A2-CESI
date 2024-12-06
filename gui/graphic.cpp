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
    // Obtient la résolution de l'écran du bureau (le mode vidéo du bureau)
    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();

    // Définit la largeur et la hauteur de la fenêtre, en laissant un peu de marge
    int screenWidth = desktop.width - 10;    // 10 pixels de marge
    int screenHeight = desktop.height - 200; // 200 pixels de marge en bas

    // Initialise les variables associées à la vitesse, à la pause et aux informations de développement
    speed = 10;      // Vitesse par défaut de l'animation
    pause = false;   // État initial de pause : non-pausé
    devInfo = false; // Afficher les informations de développement par défaut

    // Définit la taille de la cellule (initialisée à 8 pixels)
    int tempCellSize = 8; // Cette valeur pourrait être changée (par exemple 4)

    // Calcule la largeur et la hauteur de la grille en fonction de la taille de l'écran et de la taille des cellules
    gridWidth = screenWidth / tempCellSize;
    gridHeight = screenHeight / tempCellSize;

    // Calcule la taille des cellules en prenant le plus petit facteur pour que la grille tienne dans la fenêtre
    cellSize = std::min(screenWidth / gridWidth, screenHeight / gridHeight);

    // Crée la fenêtre avec les dimensions calculées
    window.create(sf::VideoMode(screenWidth, screenHeight + 100), "CESI GAMING");

    // Positionne la fenêtre en haut à gauche de l'écran
    window.setPosition(sf::Vector2i(0, 0));

    // Limite le taux de rafraîchissement à 30 FPS pour une animation fluide
    window.setFramerateLimit(30);

    // Crée un objet Grid avec les dimensions calculées (largeur et hauteur de la grille)
    grid = Grid(gridWidth, gridHeight);

    // Initialise la grille avec les valeurs de départ
    grid.initializeGrid();
}

Graphic::~Graphic()
{
}

void Graphic::renderGrid()
{
    // Récupère le nombre de threads supportés par le processeur
    int numThreads = thread::hardware_concurrency();

    // Crée un vecteur pour stocker les objets thread
    vector<thread> threads;

    // Divise la largeur de la grille par le nombre de threads
    int sectionWidth = gridWidth / numThreads;

    // Lance un thread pour chaque section de la grille
    for (int i = 0; i < numThreads; ++i)
    {
        // Calcule les indices de la section à traiter pour chaque thread
        int startX = i * sectionWidth;
        // Si c'est le dernier thread, il va traiter jusqu'à la fin de la grille
        int endX = (i == numThreads - 1) ? gridWidth : (i + 1) * sectionWidth;

        // Lance un thread qui appellera la méthode updateSection pour une section de la grille
        threads.emplace_back(&Graphic::updateSection, this, startX, endX, 0, gridHeight);
    }

    // Attends que tous les threads terminent leur travail
    for (auto &thread : threads)
    {
        thread.join();
    }

    // Cette boucle s'assure que les cellules sont mises à jour après avoir utilisé les threads
    for (int x = 0; x < gridWidth; ++x)
    {
        for (int y = 0; y < gridHeight; ++y)
        {
            // Appelle la méthode update() de chaque cellule de la grille pour actualiser son état
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
    // Définition de la taille de la barre latérale
    float sidebarHeight = 100.0f;              // Hauteur de la barre latérale
    float sidebarWidth = gridWidth * cellSize; // Largeur de la barre latérale (égale à la largeur de la grille en pixels)

    // Création du rectangle représentant la barre latérale
    sf::RectangleShape sidebar(sf::Vector2f(sidebarWidth, sidebarHeight));
    sidebar.setFillColor(sf::Color(50, 50, 50));   // Définition de la couleur de fond (gris sombre)
    sidebar.setPosition(0, gridHeight * cellSize); // Positionne la barre latérale juste sous la grille
    window.draw(sidebar);                          // Dessine la barre latérale dans la fenêtre

    // Définition des dimensions et marges des boutons
    float buttonWidth = 120;                   // Largeur des boutons
    float buttonHeight = 40;                   // Hauteur des boutons
    float margin = 20;                         // Marge entre les boutons
    float startX = 10;                         // Position initiale en X pour les boutons
    float startY = gridHeight * cellSize + 30; // Position initiale en Y pour les boutons (juste en dessous de la grille)

    // Création et affichage des boutons avec leurs textes

    // Bouton "Pause/Play"
    sf::RectangleShape button1(sf::Vector2f(buttonWidth, buttonHeight));
    button1.setFillColor(sf::Color(100, 100, 100)); // Gris clair
    button1.setPosition(startX, startY);
    window.draw(button1);

    // Texte du bouton "Pause/Play"
    sf::Font font;
    if (font.loadFromFile("fonts/arial.ttf")) // Vérifie si la police Arial est chargée
    {
        sf::Text buttonText1("Pause/Play", font, 16);      // Crée le texte du bouton
        buttonText1.setFillColor(sf::Color::White);        // Texte en blanc
        buttonText1.setPosition(startX + 10, startY + 10); // Position du texte dans le bouton
        window.draw(buttonText1);                          // Dessine le texte du bouton

        // Bouton "Reset Grid"
        sf::RectangleShape button2(sf::Vector2f(buttonWidth, buttonHeight));
        button2.setFillColor(sf::Color(100, 100, 100));               // Gris clair
        button2.setPosition(startX + (buttonWidth + margin), startY); // Décalage de la position du bouton suivant
        window.draw(button2);

        // Texte du bouton "Reset Grid"

        sf::Text buttonText2("Reset Grid", font, 16);                               // Crée le texte du bouton
        buttonText2.setFillColor(sf::Color::White);                                 // Texte en blanc
        buttonText2.setPosition(startX + (buttonWidth + margin) + 10, startY + 10); // Position du texte dans le bouton
        window.draw(buttonText2);                                                   // Dessine le texte du bouton

        // Bouton "Quit"
        sf::RectangleShape button3(sf::Vector2f(buttonWidth, buttonHeight));
        button3.setFillColor(sf::Color(100, 100, 100));                   // Gris clair
        button3.setPosition(startX + 2 * (buttonWidth + margin), startY); // Décalage de la position du bouton suivant
        window.draw(button3);

        // Texte du bouton "Quit"

        sf::Text buttonText3("Quit", font, 16);                                         // Crée le texte du bouton
        buttonText3.setFillColor(sf::Color::White);                                     // Texte en blanc
        buttonText3.setPosition(startX + 2 * (buttonWidth + margin) + 10, startY + 10); // Position du texte dans le bouton
        window.draw(buttonText3);                                                       // Dessine le texte du bouton

        // Bouton "Speed-"
        sf::RectangleShape button4(sf::Vector2f(buttonWidth, buttonHeight));
        button4.setFillColor(sf::Color(100, 100, 100));                   // Gris clair
        button4.setPosition(startX + 3 * (buttonWidth + margin), startY); // Décalage de la position du bouton suivant
        window.draw(button4);

        // Texte du bouton "Speed-"

        sf::Text buttonText4("Speed-", font, 16);                                       // Crée le texte du bouton
        buttonText4.setFillColor(sf::Color::White);                                     // Texte en blanc
        buttonText4.setPosition(startX + 3 * (buttonWidth + margin) + 10, startY + 10); // Position du texte dans le bouton
        window.draw(buttonText4);                                                       // Dessine le texte du bouton

        // Bouton "Speed+"
        sf::RectangleShape button5(sf::Vector2f(buttonWidth, buttonHeight));
        button5.setFillColor(sf::Color(100, 100, 100));                   // Gris clair
        button5.setPosition(startX + 4 * (buttonWidth + margin), startY); // Décalage de la position du bouton suivant
        window.draw(button5);

        // Texte du bouton "Speed+"

        sf::Text buttonText5("Speed+", font, 16);                                       // Crée le texte du bouton
        buttonText5.setFillColor(sf::Color::White);                                     // Texte en blanc
        buttonText5.setPosition(startX + 4 * (buttonWidth + margin) + 10, startY + 10); // Position du texte dans le bouton
        window.draw(buttonText5);                                                       // Dessine le texte du bouton

        // Bouton "Load Grid"
        sf::RectangleShape button6(sf::Vector2f(buttonWidth, buttonHeight));
        button6.setFillColor(sf::Color(100, 100, 100));                   // Gris clair
        button6.setPosition(startX + 5 * (buttonWidth + margin), startY); // Décalage de la position du bouton suivant
        window.draw(button6);

        // Texte du bouton "Load Grid"
        sf::Text buttonText6("Load Grid", font, 16);                                    // Crée le texte du bouton
        buttonText6.setFillColor(sf::Color::White);                                     // Texte en blanc
        buttonText6.setPosition(startX + 5 * (buttonWidth + margin) + 10, startY + 10); // Position du texte dans le bouton
        window.draw(buttonText6);                                                       // Dessine le texte du bouton
    }
}

void Graphic::handleMouseClick(sf::Vector2i mousePosition)
{
    // Si le clic est dans la zone de la grille (au-dessus de la barre latérale)
    if (mousePosition.y < gridHeight * cellSize)
    {
        int cellX = mousePosition.x / cellSize; // Calcul de la colonne (X) de la cellule sous le curseur
        int cellY = mousePosition.y / cellSize; // Calcul de la ligne (Y) de la cellule sous le curseur

        // Vérification que les coordonnées sont valides
        if (cellX >= 0 && cellX < gridWidth && cellY >= 0 && cellY < gridHeight)
        {
            // Si le clic est avec le bouton gauche de la souris, inverser l'état de la cellule
            if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
            {
                grid.getCell(cellX, cellY).setState(!grid.getCell(cellX, cellY).getState(), true);
                if (devInfo)
                    cout << "Cellule (" << cellX << ", " << cellY << ") inversée (clic gauche)." << endl;
            }
            // Si le clic est avec le bouton droit de la souris, définir la cellule comme un obstacle
            else if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
            {
                grid.getCell(cellX, cellY).setObstacle(!grid.getCell(cellX, cellY).getObstacle());
                if (devInfo)
                    cout << "Cellule (" << cellX << ", " << cellY << ") obstacle créé (clic droit)." << endl;
            }
        }
    }

    // Dimensions et positions des boutons dans la barre latérale
    float buttonWidth = 120;
    float buttonHeight = 40;
    float margin = 20;
    float startX = 10;
    float startY = gridHeight * cellSize + 30;

    // Bouton Pause/Play
    if (mousePosition.x >= startX && mousePosition.x <= startX + buttonWidth &&
        mousePosition.y >= startY && mousePosition.y <= startY + buttonHeight)
    {
        pause = !pause;                   // Bascule entre pause et lecture
        sf::sleep(sf::milliseconds(100)); // Petite pause pour éviter de traiter plusieurs clics en trop
        if (devInfo)
            cout << "Bouton Pause/Play cliqué !" << endl;
    }

    // Bouton Réinitialiser la grille
    if (mousePosition.x >= startX + (buttonWidth + margin) && mousePosition.x <= startX + (buttonWidth + margin) + buttonWidth &&
        mousePosition.y >= startY && mousePosition.y <= startY + buttonHeight)
    {
        grid.initializeGrid(); // Réinitialise la grille
        if (devInfo)
            cout << "Bouton Réinitialiser cliqué !" << endl;
    }

    // Bouton Quitter
    if (mousePosition.x >= startX + 2 * (buttonWidth + margin) && mousePosition.x <= startX + 2 * (buttonWidth + margin) + buttonWidth &&
        mousePosition.y >= startY && mousePosition.y <= startY + buttonHeight)
    {
        window.close(); // Ferme la fenêtre
        if (devInfo)
            cout << "Bouton Quitter cliqué !" << endl;
    }

    // Bouton Vitesse + (accélérer)
    if (mousePosition.x >= startX + 3 * (buttonWidth + margin) && mousePosition.x <= startX + 3 * (buttonWidth + margin) + buttonWidth &&
        mousePosition.y >= startY && mousePosition.y <= startY + buttonHeight)
    {
        speed = std::min(speed * 10, 2000); // Augmente la vitesse de la simulation, jusqu'à une limite
        sf::sleep(sf::milliseconds(100));   // Petite pause pour éviter les clics répétés
        if (devInfo)
            cout << "Vitesse +, nouvelle vitesse: " << speed << "ms" << endl;
    }

    // Bouton Vitesse - (ralentir)
    if (mousePosition.x >= startX + 4 * (buttonWidth + margin) && mousePosition.x <= startX + 4 * (buttonWidth + margin) + buttonWidth &&
        mousePosition.y >= startY && mousePosition.y <= startY + buttonHeight)
    {
        speed = std::max(speed / 10, 1);  // Diminue la vitesse de la simulation, avec une limite minimale
        sf::sleep(sf::milliseconds(100)); // Petite pause pour éviter les clics répétés
        if (devInfo)
            cout << "Vitesse -, nouvelle vitesse: " << speed << "ms" << endl;
    }

    // Bouton Charger la grille
    if (mousePosition.x >= startX + 5 * (buttonWidth + margin) && mousePosition.x <= startX + 5 * (buttonWidth + margin) + buttonWidth &&
        mousePosition.y >= startY && mousePosition.y <= startY + buttonHeight)
    {
        std::string filename;
        cout << "Entrez le chemin du fichier à charger : ";
        cin >> filename;                        // Demande à l'utilisateur de saisir le chemin du fichier
        grid.loadGridFromFile(filename, false); // Charge la grille depuis le fichier
        cout << "Grille chargée depuis : " << filename << endl;
    }
}
