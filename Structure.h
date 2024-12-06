#ifndef STRUCTURE_H
#define STRUCTURE_H

#include "Bouton.h"
#include "FontManager.h"

#include "menu.h"

class Structure : public Button {

private:
    Menu* menu;
    bool isClickedFlag = false; // Ajout d'un flag pour savoir si le bouton a été cliqué

public:
    Structure(float x, float y, float width, float height, const std::string& text)
        : Button(x, y, width, height, text) {}

    void onClick() override {
        std::cout << "Structure clicked" << std::endl;
        menu->open();
        isClickedFlag = true; // Le bouton est cliqué
    }

    bool getIsClicked() { return isClickedFlag; } // Méthode pour obtenir l'état du clic
    void setIsClicked(bool state) { isClickedFlag = state; } // Méthode pour réinitialiser le clic
};

#endif // STRUCTURE_H
