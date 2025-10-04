#include <iostream>
#include <vector>
#include "Map.h" 

void testLoadMaps() {
    MapLoader loader;

//Map files to validate from the Conquest game source files 
    std::vector<std::string> testFiles = {
        "Map/Asia.map",
        "Map/Europe.map",
        //Invalid map file I created for testing 
        "Map/Invalid.map",
        "Map/canada.map"
    };

    for (const auto& file : testFiles) {
        //Parse and validate the files 
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
//Test the maps
#ifndef MAIN_DRIVER_INCLUDED
int main() {
    testLoadMaps();
    return 0;
}
#endif
