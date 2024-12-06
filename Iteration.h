#ifndef ITERATION_H
#define ITERATION_H

#include "FontManager.h"
#include "Bouton.h"

class Iteration : public Button {

public:
    Iteration(float x, float y, float width, float height, const std::string& text)
        : Button(x, y, width, height, text) {}

    void onClick() override {
        std::cout << "Iteration clicked" << std::endl;
    }
};

#endif // ITERATION_H
