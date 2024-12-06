#ifndef CELL_HPP
#define CELL_HPP

class Cell
{
private:
    bool isAlive;
    bool nextState;
    bool isObstacle;
    int x;
    int y;

public:
    Cell();
    Cell(bool isAlive, bool isObstacle, int x, int y);
    ~Cell();
    int getX();
    int getY();
    bool getState();
    void setState(bool state, bool force);
    void update();
};

#endif // CELL_HPP
