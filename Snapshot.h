#ifndef SNAPSHOT_H
#define SNAPSHOT_H

#include "Bouton.h"
#include "FontManager.h"


class Snapshot : public Button {

public:
    Snapshot(float x, float y, float width, float height, const std::string& text)
        : Button(x, y, width, height, text) {}

    void onClick() override {
        std::cout << "Snapshot clicked" << std::endl;
    }
};

#endif // SNAPSHOT_H
