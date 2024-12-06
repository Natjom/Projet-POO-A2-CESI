#include "game.hpp"
#include "../gui/graphic.hpp"

#include <sys/stat.h>
#include <sstream>
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

        // Créer la grille et charger le fichier d'entrée
        Grid grid(10, 10); // Taille par défaut, sera écrasée par le contenu du fichier
        grid.loadGridFromFile(inputFilename, true);

        // Créer le répertoire de sortie sous "./exports/"
        string outputDir = "exports/" + inputFilename + "_out";

        // Créer le répertoire si il n'existe pas déjà
        int dirCreateStatus = mkdir(outputDir.c_str(), 0777); // Créer le dossier avec les permissions 0777

        // Vérifier si le répertoire a été créé avec succès
        if (dirCreateStatus == -1)
        {
            if (errno == EEXIST)
            {
                cout << "Le dossier existe déjà : " << outputDir << endl;
            }
            else
            {
                perror("Erreur lors de la création du dossier");
                return;
            }
        }

        int iterations = 0;
        string command;

        while (true)
        {
            // Affichage du menu principal
            cout << "\n--- Menu Console ---\n";
            cout << "1. Avancer de n itérations\n";
            cout << "2. Exporter l'état de la grille\n";
            cout << "3. Quitter\n";
            cout << "Commande: ";
            cin >> command;

            if (command == "1") // Avancer de n itérations
            {
                cout << "Combien d'itérations voulez-vous avancer ? ";
                int tmp;
                cin >> tmp;

                // Vérifier que le nombre d'itérations est valide
                if (tmp <= 0)
                {
                    cout << "Le nombre d'itérations doit être supérieur à 0.\n";
                    continue;
                }
                iterations += tmp;

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

                    // Nom du fichier d'itération
                    stringstream ss;
                    ss << outputDir << "/iteration_" << (i + 1);
                    string iterationFilename = ss.str();

                    // Exporter l'état de la grille à chaque itération
                    grid.Export(iterationFilename); // Exporter la grille avec le nom d'itération
                    cout << "Itération " << i + 1 << " terminée. Fichier exporté sous : " << iterationFilename << endl;
                }
            }
            else if (command == "2") // Exporter l'état de la grille après toutes les itérations
            {
                // Exporter la dernière itération avec un nom basé sur l'itération finale
                stringstream ss;

                ss << "./exports/" << inputFilename << "_out" << "/iteration_" << iterations;
                string finalIterationFilename = ss.str();
                grid.Export(finalIterationFilename); // Exporter la grille avec le nom d'itération final
                cout << "État final exporté sous : " << finalIterationFilename << endl;
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
