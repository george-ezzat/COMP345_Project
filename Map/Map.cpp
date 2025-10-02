#include "Map.h"
#include <fstream>
#include <sstream>
#include <queue>
#include <unordered_set>
#include <unordered_map>

//Signle territory in the map that has continent, owner and army
Territory::Territory(int id, const std::string& name, Continent* continent)
    : id(id), name(name), continent(continent), owner(nullptr), armies(0) {}

//Getters
int Territory::getId() const { return id; }
std::string Territory::getName() const { return name; }
Continent* Territory::getContinent() const { return continent; }

void Territory::addAdjacentTerritory(Territory* t) { adjacents.push_back(t); }
const std::vector<Territory*>& Territory::getAdjacents() const { return adjacents; }

void Territory::setOwner(Player* p) { owner = p; }
Player* Territory::getOwner() const { return owner; }
void Territory::setArmies(int n) { armies = n; }
int Territory::getArmies() const { return armies; }

//Single continent in the map that contains multiple territories 
Continent::Continent(const std::string& name) : name(name) {}
std::string Continent::getName() const { return name; }

//Add territories to the continent 
void Continent::addTerritory(Territory* t) { territories.push_back(t); }
const std::vector<Territory*>& Continent::getTerritories() const { return territories; }

//Full map with continent and territories 
Map::Map() {}
//To clean up memory when a map object is deleted 
Map::~Map() {
    for (auto t : territories) delete t;
    for (auto c : continents) delete c;
}

void Map::addContinent(Continent* c) { continents.push_back(c); }
void Map::addTerritory(Territory* t) { territories.push_back(t); }

//Get a territory by its id 
Territory* Map::getTerritory(int id) const {
    for (auto t : territories) {
        if (t->getId() == id) return t;
    }
    return nullptr;
}


const std::vector<Continent*>& Map::getContinents() const { return continents; }
const std::vector<Territory*>& Map::getTerritories() const { return territories; }

//Validate the map to check conectivity with territories and continents 
bool Map::validate() const {
    return isConnectedGraph() && continentsAreConnected() && territoriesHaveUniqueContinent();
}

//Check if the all the territories are connected 
static bool isTerritoriesConnected(const std::vector<Territory*>& nodes) {
    if (nodes.empty()) return true;
    std::unordered_set<int> visited;
    std::queue<Territory*> q;
    q.push(nodes[0]);
    visited.insert(nodes[0]->getId());

    while (!q.empty()) {
        Territory* cur = q.front(); q.pop();
        for (auto n : cur->getAdjacents()) {
            if (visited.count(n->getId()) == 0) {
                visited.insert(n->getId());
                q.push(n);
            }
        }
    }
    //Graph is connected if we visted all the nodes
    return visited.size() == nodes.size();
}

//Check if the entire map is connected 
bool Map::isConnectedGraph() const {
    return isTerritoriesConnected(territories);
}

//Check if each continent is connected 
bool Map::continentsAreConnected() const {
    for (auto c : continents) {
        if (!isTerritoriesConnected(c->getTerritories())) return false;
    }
    return true;
}

//Check if all the territories have a unique continent 
bool Map::territoriesHaveUniqueContinent() const {
    std::unordered_set<int> ids;
    for (auto c : continents) {
        for (auto t : c->getTerritories()) {
            if (!ids.insert(t->getId()).second) return false;
        }
    }
    return true;
}

//Loads the maps from a .map file
MapLoader::MapLoader() {}

//Call validate funcition to validatde each .map file
Map* MapLoader::loadMap(const std::string& filename) {
    Map* map = parseFile(filename);
    if (map && map->validate()) {
        return map;
    } else {
    std::cout << "Invalid map file: " << filename << "\n";
    delete map;
    return nullptr;}
}

//Parse the .map files
Map* MapLoader::parseFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) 
        return nullptr;

    Map* map = new Map();
    std::string line;
    bool inContinents = false;
    bool inTerritories = false;

    std::unordered_map<std::string, Continent*> continentMap;
    std::unordered_map<std::string, Territory*> territoryMap;

    //read map files 
    while (std::getline(file, line)) {
        if (line == "[Continents]") {
            inContinents = true; 
            inTerritories = false;
            continue;
        } else if (line == "[Territories]") {
            inContinents = false; 
            inTerritories = true;
            continue;
        }

        if (inContinents && !line.empty()) {
            std::istringstream ss(line);
            std::string name;
            int bonus;
                if (!(ss >> name >> bonus)) {
                std::cout << "Error: Invalid continent line: " << line << std::endl;
                continue;
            }
            Continent* c = new Continent(name);
            map->addContinent(c);
            continentMap[name] = c;
        }

        if (inTerritories && !line.empty()) {
            std::istringstream ss(line);
            std::string name, continentName;
            int x, y;
              if (!(ss >> name >> x >> y >> continentName)) {
                std::cout << "Error: Invalid territory line: " << line << std::endl;
                continue;
            }
            //Check if continent exists in continentMap 
            if (continentMap.find(continentName) == continentMap.end()) {
                std::cout << "Error: Continent not found for territory " << name << std::endl;
                delete map;
                return nullptr; 
            }

            Territory* t = new Territory((int)map->getTerritories().size(), name, continentMap[continentName]);
            continentMap[continentName]->addTerritory(t);
            map->addTerritory(t);
            territoryMap[name] = t;

           
        }
    }
    return map;
}
