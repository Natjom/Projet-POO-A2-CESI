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
    Cell();
    Cell(bool isAlive, int x, int y);
    ~Cell() {};
    int getX() const { return x; }
    int getY() const { return y; }
    bool getState();
    void setState(bool state, bool force);
    void update();
};

#endif // CELL_H
