#include <string>
#include <vector>
#include <iostream>
#include "Orders/Orders.h"
#include "Cards/Cards.h"
#include "Map/Map.h"
using namespace std;

class Player {
    private:
        string name;
        vector<Territory*> territories;
        Hand* hand;
        OrdersList* ordersList;

    public:
        Player(std::string name);
        ~Player();

        string getName();
        vector<Territory*> getTerritories();
        Hand* getHand();
        OrdersList* getOrdersList();

        void addTerritory(Territory* t);
        void setHand(Hand* h);

        vector<Territory*> toDefend();        
        vector<Territory*> toAttack();        
        void issueOrder();                         
};
