#include <SFML/Graphics.hpp>
#include "Slider.h"
#include "StartStop.h"
#include "Reset.h"
#include "Structure.h"
#include "Structure1.h"
#include "Structure2.h"
#include "Structure3.h"
#include "Snapshot.h"
#include "Iteration.h"
#include "quitter.h"
#include "BoutonParametres.h"
#include "Parametre1.h"
#include "Parametre2.h"
#include "Parametre3.h"

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "Slider et Bouton");
    window.setFramerateLimit(60);

    // Chargement de la police
    if (!globalFont.loadFromFile("fonts/WinterLemon.ttf"))
    {
        std::cerr << "Erreur : impossible de charger la police." << std::endl;
    }
    else
    {
        globalFont.loadFromFile("fonts/WinterLemon.ttf");
    }

    // Créer les boutons
    Slider slider(200, 520, 400, 20); // Coordonnées (x,y) , Longeur, Hauteur, "Text"
    StartStop StartStop(15, 550, 105, 40, "Start");
    Reset reset(125, 550, 105, 40, "Reset");
    Structure struc(235, 550, 105, 40, "Structures");
    Snapshot snap(345, 550, 105, 40, "Snapshot");
    Iteration iter(455, 550, 105, 40, "Iteration");
    BoutonParametres param(565, 550, 105, 40, "Parametres");
    Quitter quit(675, 550, 105, 40, "Quitter");

    // Vecteurs pour les menus
    std::vector<BoutonStructure *> structureButtons;
    std::vector<BoutonParametres *> ParametreButtons;

    // Etats pour les menus
    bool displayStructureButtons = false;
    bool displayParametreButtons = false;

    // Boutons pour le menu Structure
    Structure1 *bouton1 = nullptr;
    Structure2 *bouton2 = nullptr;
    Structure3 *bouton3 = nullptr;

    // Boutons pour le menu Parametre
    Parametre1 *boutonP1 = nullptr;
    Parametre2 *boutonP2 = nullptr;
    Parametre3 *boutonP3 = nullptr;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }

            // Gérer les événements de chaque bouton
            if (!displayStructureButtons && !displayParametreButtons)
            {
                slider.handleMouseEvent(event, window);
                StartStop.handleMouseEvent(event, window);
                reset.handleMouseEvent(event, window);
                snap.handleMouseEvent(event, window);
                iter.handleMouseEvent(event, window);
                quit.handleMouseEvent(event, window);
                param.handleMouseEvent(event, window);
                struc.handleMouseEvent(event, window);
            }
            else if (displayStructureButtons && !displayParametreButtons)
            {
                struc.handleMouseEvent(event, window);
            }
            else if (!displayStructureButtons && displayParametreButtons)
            {
                param.handleMouseEvent(event, window);
            }

            // Vérifier si le bouton Structure est cliqué pour ouvrir le menu structure
            if (event.type == sf::Event::MouseButtonPressed && struc.getIsClicked())
            {
                displayStructureButtons = !displayStructureButtons;
                if (displayStructureButtons && bouton1 == nullptr)
                {
                    bouton1 = new Structure1(300, 200, 150, 50, "Structure I");
                    bouton2 = new Structure2(300, 270, 150, 50, "Structure II");
                    bouton3 = new Structure3(300, 340, 150, 50, "Structure III");

                    structureButtons.push_back(bouton1);
                    structureButtons.push_back(bouton2);
                    structureButtons.push_back(bouton3);
                }
            }

            // Gérer les événements des boutons Structure si le menu est ouvert
            if (displayStructureButtons)
            {
                for (auto *button : structureButtons)
                {
                    button->handleMouseEvent(event, window);
                }
            }
            // Réinitialiser les clics après un relâchement de la souris
            if (event.type == sf::Event::MouseButtonReleased)
            {
                struc.setIsClicked(false);
                for (auto *button : structureButtons)
                {
                    button->setIsClicked(false);
                }
            }

            // Gérer les événements pour le bouton Parametre
            if (event.type == sf::Event::MouseButtonPressed && param.getIsClicked())
            {
                displayParametreButtons = !displayParametreButtons;
                if (displayParametreButtons && boutonP1 == nullptr)
                {
                    boutonP1 = new Parametre1(300, 200, 150, 50, "Minecraft");
                    boutonP2 = new Parametre2(300, 270, 150, 50, "WinterLemon");
                    boutonP3 = new Parametre3(300, 340, 150, 50, "NeutronsDemo");

                    ParametreButtons.push_back(boutonP1);
                    ParametreButtons.push_back(boutonP2);
                    ParametreButtons.push_back(boutonP3);
                }
            }

            // Gérer les événements des boutons Parametre si le menu est ouvert
            if (displayParametreButtons)
            {
                for (auto *buttonP : ParametreButtons)
                {
                    buttonP->handleMouseEvent(event, window);
                }
            }

            // Réinitialiser les clics après un relâchement de la souris
            if (event.type == sf::Event::MouseButtonReleased)
            {
                param.setIsClicked(false);
                for (auto *buttonP : ParametreButtons)
                {
                    buttonP->setIsClicked(false);
                }
            }
        }

        // Mettre à jour le slider
        slider.update(window);

        // Effacer l'écran
        window.clear();

        // Rendre les boutons principaux
        slider.render(window);
        reset.render(window);
        StartStop.render(window);
        struc.render(window);
        snap.render(window);
        iter.render(window);
        quit.render(window);
        param.render(window);

        // Rendre les boutons Structure si le menu est ouvert
        if (displayStructureButtons)
        {
            for (auto *button : structureButtons)
            {
                button->render(window);
            }
        }

        // Rendre les boutons Parametre si le menu est ouvert
        if (displayParametreButtons)
        {
            for (auto *buttonP : ParametreButtons)
            {
                buttonP->render(window);
            }
        }

        window.display();

        // Afficher la valeur du slider lorsqu'elle change
        if (slider.valueChanged())
        {
            std::cout << "Speed: " << slider.getValue() << "%" << std::endl;
        }
    }

    // Libérer la mémoire des boutons
    for (auto *button : structureButtons)
    {
        delete button;
    }

    for (auto *buttonP : ParametreButtons)
    {
        delete buttonP;
    }

    return 0;
}
