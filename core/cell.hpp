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
    ~Cell() = default;
    int getX() { return x; }
    int getY() { return y; }
    void setX(int x) { this -> x = x; }
    void setY(int y) { this -> y = y; }
    bool getObstacle() { return isObstacle; }
    void setObstacle(bool state) { isObstacle = state; }
    bool getState() { return isAlive; }
    void setState(bool state, bool force);
    void update() { isAlive = nextState; }
};

#endif // CELL_HPP
