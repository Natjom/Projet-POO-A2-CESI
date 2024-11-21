#include <SFML/Graphics.hpp>
#include <vector>
#include <ctime>
#include <cstdlib>

#include "map.h"
#include "cell.h"


int main() {
    // Map map(10, 100);
    Map map(5, 10);
    map.initializeGrid();
    map.start();
    return 0;
}
