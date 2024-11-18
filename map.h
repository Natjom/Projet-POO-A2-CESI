#ifndef MAP_H
#define MAP_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <ctime>
#include <cstdlib>

using namespace std;

class Map
{
private:
    int cellSize;
    int gridWidth;
    int gridHeight;
    vector<vector<int>> grid;
    sf::RenderWindow window;
public:
    //Constructeurs
    Map();
    Map(int cellSize, int gridWidth, int gridHeight);


    //Methodes

    void initializeGrid();
    void renderGrid(sf::RenderWindow &window);
    void start();
};

#endif // MAP_H
