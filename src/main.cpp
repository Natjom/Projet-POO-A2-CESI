#include <SFML/Graphics.hpp>
#include <vector>
#include <ctime>
#include <cstdlib>


#include "map.hpp"
#include "cell.hpp"


int main() {
    Map map;
    map.initializeGrid();
    map.start();
    return 0;
}
