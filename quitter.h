#ifndef QUITER_H
#define QUITER_H

#include "Bouton.h"
#include "FontManager.h"


class Quitter : public Button {

public:
    Quitter(float x, float y, float width, float height, const std::string& text)
        : Button(x, y, width, height, text) {}

    void onClick() override {
        std::cout << "Quitter clicked" << std::endl;
    }
};

#endif // QUITER_H
