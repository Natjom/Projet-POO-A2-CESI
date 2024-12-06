#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath> 
#include"FontManager.h"



class Slider {
private:
    sf::RectangleShape bar;     // La barre de fond
    sf::RectangleShape knob;    // Le curseur
    sf::Text text;              // Text a afficher au dessus le curseur
    sf::Font font;              // Font de ce text
    float minValue, maxValue;   // Plage de valeurs (0-100)
    float value;                // Valeur actuelle
    bool isDragging;            // Indique si le bouton de la souris glisse sur le curseur
    bool hasChanged;            // Indique si la valeur a changé récemment

public:
    Slider(float x, float y, float width, float height, float minVal = 0.f, float maxVal = 100.f)
        : minValue(minVal), maxValue(maxVal), value(minVal), isDragging(false), hasChanged(false) 
        {

        // Chargement de la police
        if (!globalFont.loadFromFile("fonts/WinterLemon.ttf")) {
            std::cerr << "Erreur : impossible de charger la police." << std::endl;
        }
        else {
            globalFont.loadFromFile("fonts/WinterLemon.ttf");
        }

        // Initialisation de la barre
        bar.setPosition(x, y);
        bar.setSize({width, height});
        bar.setFillColor(sf::Color(100, 100, 100)); // gris

        // Initialisation du curseur
        knob.setSize({height, height}); // carré
        knob.setFillColor(sf::Color::Red); // rouge
        knob.setPosition(x, y);
        

        // Initialisation du texte
        text.setFont(globalFont);
        text.setString("Speed");
        text.setCharacterSize(20); // Taille du texte
        text.setFillColor(sf::Color::White); // Couleur du texte

        }

    void handleMouseEvent(const sf::Event& event, const sf::RenderWindow& window) 
    {
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);

        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) 
        {
            // Si on clique sur le curseur
            if (knob.getGlobalBounds().contains(mousePos.x, mousePos.y)) 
            {
                isDragging = true;
            }
        }

        if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) 
        {
            isDragging = false; // Arrêter le glissement
        }
    }

    void update(const sf::RenderWindow& window) 
    {
        if (isDragging) {
            // Suivre la souris sur l'axe X
            sf::Vector2i mousePos = sf::Mouse::getPosition(window);
            float newX = std::clamp(static_cast<float>(mousePos.x), bar.getPosition().x, bar.getPosition().x + bar.getSize().x - knob.getSize().x);
            if (newX != knob.getPosition().x) 
            { // Vérifier si la position a changé
                hasChanged = true;
                knob.setPosition(newX, knob.getPosition().y);

                // Calculer la nouvelle valeur
                float relativePosition = (knob.getPosition().x - bar.getPosition().x) / (bar.getSize().x - knob.getSize().x);
                value = minValue + relativePosition * (maxValue - minValue);
            }
        }
    }

    float getValue() const 
    {
        return value;
    }

    bool valueChanged() 
    {
        if (hasChanged) 
        {
            hasChanged = false; // Réinitialiser l'indicateur après lecture
            return true;
        }
        return false;
    }

    void render(sf::RenderWindow& window) 
    {
    window.draw(bar);
    window.draw(knob);

    // Placer le texte juste au-dessus du curseur

    sf::Vector2f knobPos = knob.getPosition();
    text.setPosition(knobPos.x + knob.getSize().x / 2 - text.getGlobalBounds().width / 2, knobPos.y - 30);

    window.draw(text); // Afficher le texte
    }
};
