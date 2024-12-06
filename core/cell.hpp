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
    // Constructeurs
    Cell();
    Cell(bool isAlive, bool isObstacle, int x, int y);

    // Destructeur (déjà défini par défaut, à laisser si nécessaire)
    ~Cell() = default;

    // Accesseurs
    int getX() const { return x; }
    int getY() const { return y; }
    bool getObstacle() const { return isObstacle; }
    bool getState() const { return isAlive; }

    // Mutateurs
    void setX(int x) { this->x = x; }
    void setY(int y) { this->y = y; }
    void setObstacle(bool state) { isObstacle = state; }
    void setState(bool state, bool force = false);  // Force option
    void update() { isAlive = nextState; }

    // Méthodes supplémentaires
    void resetState() { nextState = isAlive; }  // Peut être utile pour la réinitialisation
};

#endif // CELL_HPP
