#include "cell.h"

Cell::Cell() : isAlive(false), x(0), y(0) {}

// Constructeur paramétré
Cell::Cell(bool state, int x, int y) : isAlive(state), x(x), y(y) {}

// Accesseurs
bool Cell::getState() { return isAlive; }
void Cell::setState(bool state) { isAlive = state; }
