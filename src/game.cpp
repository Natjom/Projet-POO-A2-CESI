#include "game.hpp"
#include "../gui/graphic.hpp"

#include <iostream>

using namespace std;

Game::Game()
{
    Start();
}

Game::~Game()
{
}

void Game::Start()
{
    int choix;

    cout << "Faites un choix (1 pour mode graphique, 2 pour mode console): ";
    cin >> choix;

    if (choix == 1) // mode graphique
    {
        Graphic graphic;
        graphic.start();
    }
    else if (choix == 2) // mode console
    {
        string inputFilename;
        cout << "Entrez le nom du fichier d'entrée (doit être dans 'data/'): ";
        cin >> inputFilename;
        cout << inputFilename << endl;

        // Créer la grille et charger le fichier d'entrée
        Grid grid(10, 10); // Taille par défaut, sera écrasée par le contenu du fichier
        grid.loadGridFromFile(inputFilename, true);

        string outputFilename = inputFilename + "_out"; // Nom de fichier d'export par défaut
        cout << outputFilename << endl;
        int iterations;
        string command;

        while (true)
        {
            // Affichage du menu principal
            cout << "\n--- Menu Console ---\n";
            cout << "1. Avancer de n itérations\n";
            cout << "2. Exporter l'état de la grille (nom par défaut : " << outputFilename << ")\n";
            cout << "3. Quitter\n";
            cout << "Commande: ";
            cin >> command;

            if (command == "1") // Avancer de n itérations
            {
                cout << "Combien d'itérations voulez-vous avancer ? ";
                cin >> iterations;

                // Vérifier que le nombre d'itérations est valide
                if (iterations <= 0)
                {
                    cout << "Le nombre d'itérations doit être supérieur à 0.\n";
                    continue;
                }

                // Avancer la grille de 'iterations' pas
                for (int i = 0; i < iterations; ++i)
                {
                    for (int x = 0; x < grid.getWidth(); ++x)
                    {
                        for (int y = 0; y < grid.getHeight(); ++y)
                        {
                            grid.rule(x, y); // Appliquer les règles
                        }
                    }
                    grid.update(); // Mettre à jour l'état de la grille
                    cout << "Itération " << i + 1 << " terminée.\n";
                }
            }
            else if (command == "2") // Exporter l'état de la grille
            {
                grid.Export(outputFilename); // Exporter la grille
            }
            else if (command == "3") // Quitter
            {
                cout << "Fin du programme. Merci d'avoir joué !\n";
                break; // Sortir de la boucle et terminer le mode console
            }
            else
            {
                cout << "Commande invalide. Essayez encore.\n";
            }
        }
    }
    else
    {
        cout << "Choix invalide." << endl;
    }
}
