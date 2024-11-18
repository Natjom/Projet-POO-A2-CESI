#ifndef CELL_H
#define CELL_H

class Cell
{
private:
    bool isAlive;
    int x;
    int y;
public:
    //Constructeurs
    Cell();
    Cell(bool isAlive, int x, int y);
    
    //Destructeur
    ~Cell() {};

    //Methdodes
    bool getState();
};

#endif // CELL_H
