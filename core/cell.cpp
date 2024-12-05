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

Cell::~Cell()
{
}

int Cell::getX()
{
    return x;
}

int Cell::getY()
{
    return y;
}

bool Cell::getState()
{
    return isAlive;
}

void Cell::setState(bool state, bool force)

{
    nextState = state;
    if (force)
    {
        isAlive = state;
    }
}

void Cell::update()
{
    isAlive = nextState;
}
