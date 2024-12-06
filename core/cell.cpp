#include "cell.hpp"

Cell::Cell()
    : isAlive(false), nextState(false), isObstacle(false), x(0), y(0)
{
}

Cell::Cell(bool state, bool isObstacle, int x, int y)
    : isAlive(state), nextState(state), isObstacle(isObstacle), x(x), y(y)
{
}

void Cell::setState(bool state, bool force)
{
    nextState = state;
    if (force)
    {
        isAlive = state;
    }
}
