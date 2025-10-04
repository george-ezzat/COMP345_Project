#pragma once
#include <string>
#include <vector>
#include <iostream>
#include "../Orders/Orders.h"
#include "../Cards/Cards.h"
#include "../Map/Map.h"
using namespace std;

class Player {
    private:
        std::string name; 
        std::vector<Territory*>* territories; // Should be a pointer
        WarzoneCard::Hand* hand;
        OrdersList* ordersList;

    public:
        Player(std::string name);
        ~Player();

        std::string getName();
        std::vector<Territory*>* getTerritories();
        WarzoneCard::Hand* getHand();
        OrdersList* getOrdersList();

        void addTerritory(Territory* t);
        void setHand(WarzoneCard::Hand* h);

        vector<Territory*>* toDefend();        
        vector<Territory*>* toAttack();        
        void issueOrder(string orderType);                         
};
