#ifndef GRAPHIC_HPP
#define GRAPHIC_HPP

#include <SFML/Graphics.hpp>
#include "../core/grid.hpp"

class Graphic
{
private:
    int cellSize,
        gridWidth,
        gridHeight,
        speed;
    bool pause,
        devInfo;
    Grid grid;
    sf::RenderWindow window;
public:
    Graphic(/* args */);
    ~Graphic();

    void renderGrid();
    void start();
    void updateSection(int startX, int endX, int startY, int endY);
    void renderDevInfo();
    void renderSidebar();
    void handleMouseClick(sf::Vector2i mousePosition);
};

#endif // GRAPHIC_HPP