#include "cell.h"

Cell::Cell() : isAlive(false), nextState(false), x(0), y(0) {}

// Constructeur par parametres
Cell::Cell(bool state, int x, int y) : isAlive(state), nextState(state), x(x), y(y) {}

// Accesseurs
bool Cell::getState() { return isAlive; }
void Cell::setState(bool state) { nextState = state; }
void Cell::update() { isAlive = nextState; }
