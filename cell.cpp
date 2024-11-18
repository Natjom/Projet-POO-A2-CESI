#include "cell.h"

Cell::Cell() : isAlive(false), nextState(state), x(0), y(0) {}

// Constructeur paramétré
Cell::Cell(bool state, int x, int y) : isAlive(state), nextState(state), x(x), y(y) {}

// Accesseurs
bool Cell::getState() { return isAlive; }
void Cell::setState(bool state) { nextState = state; }
void Cell::update() { isAlive = nextState; }
