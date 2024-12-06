#ifndef PARAMETRE2_H
#define PARAMETRE2_H

#include "BoutonParametres.h"
#include "FontManager.h"
#include <iostream>

class Parametre2 : public BoutonParametres {
private:
    bool isClickedFlag = false; // Flag spécifique pour ce bouton

public:
    Parametre2(float x, float y, float width, float height, const std::string& text)
        : BoutonParametres(x, y, width, height, text) 
        {

            if (!font.loadFromFile("fonts/WinterLemon.ttf")) {
            std::cerr << "Erreur : impossible de charger la nouvelle police." << std::endl;
            }

            // Appliquer la nouvelle police au texte
            buttonText.setFont(font);
        
            // Recalibrer la position du texte après le changement de police
            buttonText.setPosition(
                buttonShape.getPosition().x + buttonShape.getSize().x / 2.f - buttonText.getGlobalBounds().width / 2.f,
                buttonShape.getPosition().y + buttonShape.getSize().y / 2.f - buttonText.getGlobalBounds().height / 2.f + 3.f
            );

        }

    void onClick() override {
        std::cout << "BoutonParametre1 clicked" << std::endl;
        
        // Charger une nouvelle police lorsque le bouton est cliqué
        if (!globalFont.loadFromFile("fonts/WinterLemon.ttf")) {
            std::cerr << "Erreur : impossible de charger la nouvelle police." << std::endl;
        }

        // Changer la police des autres éléments de texte
        // Exemple de texte
        buttonText.setFont(globalFont);  

        isClickedFlag = true;
    }

    bool getIsClicked() const { return isClickedFlag; }
    void setIsClicked(bool state) { isClickedFlag = state; }
};

#endif // PARAMETRE2_H
