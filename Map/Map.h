#ifndef MAP_H
#define MAP_H

#include <string>
#include <vector>
#include <map>
#include <set>
#include <iostream>

class Continent;
class Player;

//Territory class (represents a territory on the map) 
class Territory {
public:
    Territory(int id, const std::string& name, Continent* continent);
    //Copy constructor 
    Territory(const Territory& other);         
    //Assignment operator  
    Territory& operator=(const Territory& other); 
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

// Continent class (holds multiple terretories)
class Continent {
public:
    Continent(const std::string& name);

    //Copy constructor
    Continent(const Continent& other); 

    //Assignment operator           
    Continent& operator=(const Continent& other); 

    //Destructor
    ~Continent() {};                                  

    std::string getName() const;
    void addTerritory(Territory* t);
    const std::vector<Territory*>& getTerritories() const;

    friend std::ostream& operator<<(std::ostream& os, const Continent& c);

private:
    std::string name;
    // pointers to territories
    std::vector<Territory*> territories; 
};

//Map class that has continents and territories 
class Map {
public:
    Map();
    // Copy constructor
    Map(const Map& other); 
    // Assignment operator          
    Map& operator=(const Map& other);
    // Destructor 
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

//MapLoader classes that loads the map files and parses the files for validation
class MapLoader {
public:
    MapLoader();
    Map* loadMap(const std::string& filename);

private:
    Map* parseFile(const std::string& filename);
};

#endif
