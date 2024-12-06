#ifndef PARAMETRES_H
#define PARAMETRES_H

#include "Bouton.h"
#include "FontManager.h"

#include <iostream>

class Parametres : public Button { //classe abstraite, les boutons du menu parametres heriterons de cette classe
private:
    bool isClickedFlag = false; // Ajout d'un flag pour savoir si le bouton a été cliqué

public:
    Parametres(float x, float y, float width, float height, const std::string& text)
        : Button(x, y, width, height, text) {}

    void onClick() override {
        isClickedFlag = true; // Le bouton est cliqué
    }

    bool getIsClicked() const { return isClickedFlag; } // Méthode pour obtenir l'état du clic
    void setIsClicked(bool state) { isClickedFlag = state; } // Méthode pour réinitialiser le clic
};

#endif // PARAMETRES_H
