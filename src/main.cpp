#include <SFML/Graphics.hpp>
#include <vector>
#include <ctime>
#include <cstdlib>


#include "map.h"
#include "cell.h"


int main() {
    Map map(5);
    map.initializeGrid();
    map.start();
    return 0;
}
