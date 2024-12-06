#ifndef BUTTON_H
#define BUTTON_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include"FontManager.h"

// Classe abstraite Button, on créera des classes filles pour chaque boutons
class Button {
protected:
    sf::RectangleShape buttonShape;
    sf::Text buttonText;
    sf::Font font;

public:
    Button(float x, float y, float width, float height, const std::string& text) 
    {
        // Initialisation de la forme du bouton
        buttonShape.setPosition(x, y);
        buttonShape.setSize({ width, height });
        buttonShape.setFillColor(sf::Color::Blue);

        // Chargement de la police
        if (!globalFont.loadFromFile("fonts/WinterLemon.ttf")) {
            std::cerr << "Erreur : impossible de charger la police." << std::endl;
        }
        else {
            font.loadFromFile("fonts/WinterLemon.ttf");
        }

        // Initialisation du texte
        buttonText.setFont(globalFont);
        buttonText.setString(text);
        buttonText.setCharacterSize(16);
        buttonText.setFillColor(sf::Color::White);
        buttonText.setPosition(
            x + width / 2.f - buttonText.getGlobalBounds().width / 2.f,
            y + height / 2.f - buttonText.getGlobalBounds().height / 2.f
        );
    }

    virtual ~Button() = default;

    // Méthode virtuelle pure à implémenter dans les classes dérivées
    virtual void onClick() = 0;

    void handleMouseEvent(const sf::Event& event, const sf::RenderWindow& window) 
    {
        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
            sf::Vector2i mousePos = sf::Mouse::getPosition(window);
            if (buttonShape.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                buttonShape.setFillColor(sf::Color::Red); // Change la couleur quand cliqué
                onClick(); // Appeler le comportement spécifique
            }
        }

        if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
            buttonShape.setFillColor(sf::Color::Blue); // Restaure la couleur
        }
    }

    void setText(const std::string& newText) //uniquemment pour StartStop, permet d'alterner le texte afficher
    {  
    buttonText.setString(newText); // Met à jour le texte

    // Recalcule la position pour centrer le texte dans le bouton
    buttonText.setPosition(
        buttonShape.getPosition().x + buttonShape.getSize().x / 2.f - buttonText.getGlobalBounds().width / 2.f,
        buttonShape.getPosition().y + buttonShape.getSize().y / 2.f - buttonText.getGlobalBounds().height / 2.f
    );
}

    void render(sf::RenderWindow& window) 
    {
        window.draw(buttonShape);
        window.draw(buttonText);
    }
};

#endif // BUTTON_H
