#include <iostream>
#include <vector>
#include "Map.h" 

void testLoadMaps() {
    MapLoader loader;

    std::vector<std::string> testFiles = {
        "Map/Asia.map",
        "Map/Europe.map",
        "Map/Invalid.map",
        "Map/canada.map"
    };

    for (const auto& file : testFiles) {
        Map* map = loader.loadMap(file); 
        if (map) {
            std::cout << "Map " << file << " is valid" << std::endl;
            std::cout << "  - Territories: " << map->getTerritories().size() << std::endl;
            std::cout << "  - Continents: " << map->getContinents().size() << std::endl;
            delete map; 
        } else {
            std::cout << "Map " << file << " is invalid" << std::endl;
        }
        std::cout << std::endl;
    }
}

#ifndef MAIN_DRIVER_INCLUDED
int main() {
    testLoadMaps();
    return 0;
}
#endif
