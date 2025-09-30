#include "Player.h"

Player::Player(std::string name) : name(name) {
    ordersList = new OrdersList();
    hand = nullptr;
}

Player::~Player() {
    delete ordersList;
    delete hand;
}

std::string Player::getName() const {
    return name;
}

std::vector<Territory*> Player::getTerritories() const {
    return territories;
}

Hand* Player::getHand() const {
    return hand;
}

OrdersList* Player::getOrdersList() const {
    return ordersList;
}

void Player::addTerritory(Territory* t) {
    territories.push_back(t);
}

void Player::setHand(Hand* h) {
    hand = h;
}

std::vector<Territory*> Player::toDefend() {
    std::cout << name << " deciding which territories to DEFEND..." << std::endl;
    return territories;
}

std::vector<Territory*> Player::toAttack() {
    std::cout << name << " deciding which territories to Attack..." << std::endl;
    std::vector<Territory*> attackList;
    attackList.push_back(new Territory("Enemy Land A"));
    attackList.push_back(new Territory("Enemy Land B"));
    return attackList;
}

void Player::issueOrder() {
    std::cout << name << " issuing an order..." << std::endl;
    Order* newOrder = new Order("Attack Order Example");
    ordersList->addOrder(newOrder);
}
