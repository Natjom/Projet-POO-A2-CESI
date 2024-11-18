#ifndef CELL_H
#define CELL_H

class Cell
{
private:
    bool isAlive;
    bool nextState;
    int x;
    int y;

public:
    // Constructeurs
    Cell();
    Cell(bool isAlive, int x, int y);

    // Destructeur
    ~Cell() {};

    int getX() const { return x; }
    int getY() const { return y; }


    // Methdodes
    bool getState();
    void setState(bool state);
    void update();
};

#endif // CELL_H
