#include <SFML/Graphics.hpp>
#include <vector>
#include <ctime>
#include <cstdlib>

#include "map.h"
#include "cell.h"


int main() {
    
    // Map map(10, 80, 80);
    Map map(3, 400, 400);

    map.initializeGrid();

    map.start();

    return 0;
}
