#ifndef RESET_H
#define RESET_H

#include "Bouton.h"
#include "FontManager.h"


class Reset : public Button {

public:
    Reset(float x, float y, float width, float height, const std::string& text)
        : Button(x, y, width, height, text) {}

    void onClick() override {
        std::cout << "Reset clicked" << std::endl;
    }
};

#endif // RESET_H
