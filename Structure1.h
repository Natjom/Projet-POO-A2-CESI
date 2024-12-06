#ifndef STRUCTURE1_H
#define STRUCTURE1_H

#include "BoutonStructure.h"
#include"FontManager.h"
#include <iostream>

class Structure1 : public BoutonStructure {
private:
    bool isClickedFlag = false; // Flag spécifique pour ce bouton

public:
    Structure1(float x, float y, float width, float height, const std::string& text)
        : BoutonStructure(x, y, width, height, text) {}

    void onClick() override {
        std::cout << "BoutonStructure1 clicked" << std::endl;
        isClickedFlag = true;
    }

    bool getIsClicked() const { return isClickedFlag; }
    void setIsClicked(bool state) { isClickedFlag = state; }
};

#endif // STRUCTURE1_H
