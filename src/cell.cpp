#include "cell.hpp"

Cell::Cell() : isAlive(false), nextState(false), x(0), y(0) {}
Cell::Cell(bool state, int x, int y) : isAlive(state), nextState(state), x(x), y(y) {}
bool Cell::getState() { return isAlive; }
void Cell::setState(bool state) { nextState = state; }
void Cell::update() { isAlive = nextState; }
