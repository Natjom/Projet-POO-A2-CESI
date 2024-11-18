#include "cell.h"


//Constructeur par defaut
Cell::Cell() 
{
    isAlive = false;
    x = -1;
    y = -1;
};
    

//Constructeur par parametres
Cell::Cell(bool isAlive, int x, int y)
{
    this->isAlive = isAlive;
    this->x = x;
    this->y = y;
};

//Methodes
bool Cell::getState() 
{
    return isAlive;
};