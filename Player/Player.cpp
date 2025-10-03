#include "Player.h"
using namespace std;

Player::Player(string name) : name(name) {
    ordersList = new OrdersList();
    hand = nullptr;
}

Player::~Player() {
    delete ordersList;
    delete hand;
}

string Player::getName() const {
    return name;
}

vector<Territory*> Player::getTerritories() const {
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

vector<Territory*> Player::toDefend() {
    cout << name << " deciding which territories to DEFEND..." ;
    return territories;
}

vector<Territory*> Player::toAttack() {
    cout << name << " deciding which territories to Attack...";
    vector<Territory*> attackList;
    attackList.push_back(new Territory("Enemy Land A"));
    attackList.push_back(new Territory("Enemy Land B"));
    return attackList;
}

void Player::issueOrder() {
    cout << name << " issuing an order...";
    Order* newOrder = new Order("Attack Order Example");
    ordersList->addOrder(newOrder);
}
