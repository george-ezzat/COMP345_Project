#include <iostream>
#include <vector>
#include "Map.h" 

void testLoadMaps() {
    MapLoader loader;

//Map files to validate from the Conquest game source files 
    std::vector<std::string> testFiles = {
        "Asia.map",
        "Europe.map",
        //Invalid map file I created for testing 
        "Invalid.map",
        "canada.map"
    };

    for (const auto& file : testFiles) {
        //Parse and validate the files 
        Map* map = loader.loadMap(file); 
        if (map) {
            std::cout << "Map " << file << " is valid\n\n";
            delete map; 
        } else {
            std::cout << "Map " << file << " is invalid\n\n";
        }
    }
}
//Test the maps
int main() {
    testLoadMaps();
    return 0;
}
