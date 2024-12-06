#include "cell.hpp"

Cell::Cell()
{
    isAlive = false;
    nextState = false;
    isObstacle = false;
    this->x = 0;
    this->y = 0;
}

Cell::Cell(bool state, bool isObstacle, int x, int y)
{
    isAlive = state;
    nextState = state;
    this->isObstacle = isObstacle;
    this->x = x;
    this->y = y;
}


void Cell::setState(bool state, bool force)

{
    nextState = state;
    if (force)
    {
        isAlive = state;
    }
}
