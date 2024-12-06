#ifndef BOUTONPARAMETRES_H
#define BOUTONPARAMETRES_H

#include "Bouton.h"
#include "menu.h"

class BoutonParametres : public Button {

private:
    Menu* menu;
    bool isClickedFlag = false; // Ajout d'un flag pour savoir si le bouton a été cliqué

public:
    BoutonParametres(float x, float y, float width, float height, const std::string& text)
        : Button(x, y, width, height, text) {}

    void onClick() override {
        std::cout << "Parametres clicked" << std::endl;
        menu->open();
        isClickedFlag = true; // Le bouton est cliqué
    }

    bool getIsClicked() { return isClickedFlag; } // Méthode pour obtenir l'état du clic
    void setIsClicked(bool state) { isClickedFlag = state; } // Méthode pour réinitialiser le clic
};

#endif // BOUTONPARAMETRES_H