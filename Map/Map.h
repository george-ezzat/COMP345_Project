#ifndef MAP_H
#define MAP_H

#include <string>
#include <vector>
#include <map>
#include <set>
#include <iostream>

class Continent;
class Player;

class Territory {
public:
    Territory(int id, const std::string& name, Continent* continent);
    Territory(const Territory& other);  // Copy constructor       
    Territory& operator=(const Territory& other);  // Assignment operator
    ~Territory() {};                              

    int getId() const;
    std::string getName() const;
    Continent* getContinent() const;

    void addAdjacentTerritory(Territory* t);
    const std::vector<Territory*>& getAdjacents() const;

    void setOwner(Player* p);
    Player* getOwner() const;
    void setArmies(int n);
    int getArmies() const;

    friend std::ostream& operator<<(std::ostream& os, const Territory& t);

private:
    int id;
    std::string name;
    Continent* continent;
    std::vector<Territory*> adjacents;
    Player* owner;
    int armies;
};

class Continent {
public:
    Continent(const std::string& name);
    Continent(const Continent& other);  // Copy constructor
    Continent& operator=(const Continent& other);  // Assignment operator
    ~Continent() {};                                  

    std::string getName() const;
    void addTerritory(Territory* t);
    const std::vector<Territory*>& getTerritories() const;

    friend std::ostream& operator<<(std::ostream& os, const Continent& c);

private:
    std::string name;
    std::vector<Territory*> territories; 
};

class Map {
public:
    Map();
    Map(const Map& other);  // Copy constructor
    Map& operator=(const Map& other);  // Assignment operator
    ~Map();                           

    void addContinent(Continent* c);
    void addTerritory(Territory* t);
    Territory* getTerritory(int id) const;

    const std::vector<Continent*>& getContinents() const;
    const std::vector<Territory*>& getTerritories() const;

    bool validate() const;

    friend std::ostream& operator<<(std::ostream& os, const Map& map);

private:
    std::vector<Continent*> continents;
    std::vector<Territory*> territories;

    bool isConnectedGraph() const;
    bool continentsAreConnected() const;
    bool territoriesHaveUniqueContinent() const;
};

class MapLoader {
public:
    MapLoader();
    Map* loadMap(const std::string& filename);

private:
    Map* parseFile(const std::string& filename);
};

#endif
