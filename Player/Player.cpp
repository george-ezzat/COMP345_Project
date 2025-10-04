#include "Player.h"

using namespace std;

Player::Player(string name) : name(name) {
    ordersList = new OrdersList();
    hand = nullptr;
    territories = new vector<Territory*>();
}

Player::~Player() {
    delete ordersList;
    delete hand;
    // Clean up territories (but don't delete the Territory objects themselves
    // as they might be owned by the game/map)
    delete territories;
}

string Player::getName() /*const */ {
    return name;
}

vector<Territory*>* Player::getTerritories() /*const */ {
    return territories;
}

WarzoneCard::Hand* Player::getHand() /*const */ {
    return hand;
}

OrdersList* Player::getOrdersList() /*const */ {
    return ordersList;
}

void Player::addTerritory(Territory* t) {
    territories->push_back(t);
}

void Player::setHand(WarzoneCard::Hand* h) {
    hand = h;
}

vector<Territory*>* Player::toDefend() {
    cout << name << " deciding which territories to DEFEND..." ;
    return territories;
}

vector<Territory*>* Player::toAttack() {
    cout << name << " deciding which territories to Attack...";
    vector<Territory*>* attackList = new vector<Territory*>();
    Continent* europe = new Continent("Europe");
    attackList->push_back(new Territory(1, "Ukrain", europe));
    attackList->push_back(new Territory(2, "Enemy Land B", europe));
    return attackList;
}

void Player::issueOrder(string orderType) {
    cout << name << " issuing an order...";
    Order* newOrder = nullptr;
    if(orderType == "Advance") {
        newOrder = new Advance();
    } else if(orderType == "Bomb") {
        newOrder = new Bomb();
    } else if(orderType == "Blockade") {
        newOrder = new Blockade();
    } else if(orderType == "Airlift") {
        newOrder = new Airlift();
    } else if(orderType == "Negotiate") {
        newOrder = new Negotiate();
    }
    ordersList->add(newOrder);
}
