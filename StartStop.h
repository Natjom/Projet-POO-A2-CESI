#ifndef STARTSTOP_H
#define STARTSTOP_H

#include "Bouton.h"
#include "FontManager.h"


class StartStop : public Button {

private:
    bool isStarted; //variable pour detecter si on a appuyé sur le bouton (pour changer affichage entre start et stop)

public:
    StartStop(float x, float y, float width, float height, const std::string& text)
        : Button(x, y, width, height, text), isStarted(false) {}

    void onClick() override {
        // Alterner entre Start et Stop
        isStarted = !isStarted;
        setText(isStarted ? "Stop" : "Start"); // Met à jour le texte du bouton
        std::cout << "Start/Stop clicked" << std::endl;
    }
};

#endif // STARTSTOP_H
