#include <SFML/Graphics.hpp>
#include <vector>
#include <ctime>
#include <cstdlib>

#include "map.h"
#include "cell.h"


int main() {
    
    Map map(2, 550, 550, 1);
    // Map map(5, 200, 200, 100);
    // Map map(10, 80, 80, 100);
    // Map map(50, 20, 20, 100);

    map.initializeGrid();

    map.start();

    return 0;
}
