#ifndef MENUPARAMETRES_H
#define MENUPARAMETRES_H

#include "Bouton.h"
#include "FontManager.h"

#include <vector>
#include <iostream>

class MenuParametres{
    private :
        std::vector<Button*> buttons; // Liste de boutons du menu
        bool isOpen;

    public:

    //Constructeur
    MenuParametres() : isOpen(false) {}

    //Methodes
    void addButton(Button* button) {
        buttons.push_back(button);
    }

    void handleMouseEvent(const sf::Event& event, sf::RenderWindow& window) {
        // Gérer les événements de clic sur les boutons du menu
        if (isOpen) {
            for (auto& button : buttons) {
                button->handleMouseEvent(event, window);
            }
        }
    }

    void render(sf::RenderWindow& window) {
        // Si le menu est ouvert, afficher les boutons
        if (isOpen) {
            for (auto& button : buttons) {
                button->render(window);
            }
        }
    }

    void open() {
        isOpen = true;
    }

    void close() {
        isOpen = false;
    }

    bool getIsOpen() const {
        return isOpen;
    }
};




#endif // MENUPARAMETRES_H