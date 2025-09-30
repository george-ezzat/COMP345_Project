#include "Player.h"
using namespace std;

void testPlayers() {
    Player* p1 = new Player("Alice");

    p1->addTerritory(new Territory("Territory 1"));
    p1->addTerritory(new Territory("Territory 2"));

    auto defendList = p1->toDefend();
    std::cout << p1->getName() << " will Defend:" <<;
    for (auto t : defendList) {
        std::cout << " - " << t->name <<;
    }

    auto attackList = p1->toAttack();
    std::cout << p1->getName() << " will Attack:" <<;
    for (auto t : attackList) {
        std::cout << " - " << t->name <<;
    }

    p1->issueOrder();
    p1->getOrdersList()->printOrders();

    delete p1;
}
