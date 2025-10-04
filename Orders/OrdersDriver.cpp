#include "Orders.h"
#include <iostream>

void testOrdersLists() {
    std::cout << "Orders minimal demo" << std::endl << std::endl;

    OrdersList list;

    // Orders with nullptr targets for now
    list.add(new Deploy(3, nullptr));
    list.add(new Advance(2, nullptr, nullptr));
    list.add(new Bomb(nullptr));
    list.add(new Blockade(nullptr));
    list.add(new Airlift(1, nullptr, nullptr));
    list.add(new Negotiate(nullptr));

    std::cout << "Initial list:\n" << list << std::endl;

    std::cout << "Move order 0 -> 2\n";
    list.move(0, 2);
    std::cout << list << std::endl;

    std::cout << "Remove order at 1\n";
    list.remove(1);
    std::cout << list << std::endl;

    std::cout << "Execute all orders:\n";
    for (Order *o : *list.getOrders()) {
        std::cout << "Executing: " << *o << std::endl;
        o->execute();
        std::cout << "  -> " << *o << std::endl;
    }

    std::cout << "\nCopied and assigned lists demo:\n";
    OrdersList copy = list;
    OrdersList assigned;
    assigned = list;
    std::cout << "Copied: \n" << copy << std::endl;
    std::cout << "Assigned: \n" << assigned << std::endl;
}

#ifndef MAIN_DRIVER_INCLUDED
int main() {
    testOrdersLists();
    return 0;
}
#endif