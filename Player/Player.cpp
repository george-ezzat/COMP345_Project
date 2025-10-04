#include "Player.h"

using namespace std;

Player::Player(const std::string& name) : name(name) {
    ordersList = new OrdersList();
    hand = nullptr;
    territories = new std::vector<Territory*>();
}

// Copy constructor
Player::Player(const Player& other) 
    : name(other.name) {
    ordersList = new OrdersList(*other.ordersList);
    hand = other.hand ? new WarzoneCard::Hand(*other.hand) : nullptr;
    territories = new std::vector<Territory*>(*other.territories);
}

// Assignment operator
Player& Player::operator=(const Player& other) {
    if (this != &other) {
        name = other.name;
        delete ordersList;
        delete hand;
        delete territories;
        
        ordersList = new OrdersList(*other.ordersList);
        hand = other.hand ? new WarzoneCard::Hand(*other.hand) : nullptr;
        territories = new std::vector<Territory*>(*other.territories);
    }
    return *this;
}

Player::~Player() {
    delete ordersList;
    delete hand;
    delete territories;
}

std::string Player::getName() const {
    return name;
}

std::vector<Territory*>* Player::getTerritories() const {
    return territories;
}

WarzoneCard::Hand* Player::getHand() const {
    return hand;
}

OrdersList* Player::getOrdersList() const {
    return ordersList;
}

void Player::addTerritory(Territory* t) {
    territories->push_back(t);
}

void Player::setHand(WarzoneCard::Hand* h) {
    hand = h;
}

std::vector<Territory*>* Player::toDefend() {
    std::cout << name << " deciding which territories to DEFEND..." << std::endl;
    return territories;
}

std::vector<Territory*>* Player::toAttack() {
    std::cout << name << " deciding which territories to Attack..." << std::endl;
    std::vector<Territory*>* attackList = new std::vector<Territory*>();
    Continent* europe = new Continent("Europe");
    attackList->push_back(new Territory(1, "Ukraine", europe));
    attackList->push_back(new Territory(2, "Enemy Land B", europe));
    return attackList;
}

void Player::issueOrder(const std::string& orderType) {
    std::cout << name << " issuing an order..." << std::endl;
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
    if (newOrder) {
        ordersList->add(newOrder);
    }
}

std::ostream& operator<<(std::ostream& os, const Player& player) {
    os << "Player(" << player.getName() 
       << ", Territories:" << player.getTerritories()->size() << ")";
    return os;
}
