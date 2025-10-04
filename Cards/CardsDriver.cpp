#include "Cards.h"
#include "../Player/Player.h"
#include "../Orders/Orders.h"
#include <iostream>

void testCards() {
    std::cout << "=== Testing Cards Module ===" << std::endl;
    
    WarzoneCard::Deck deck;
    std::cout << "Initial deck: " << deck << std::endl;
    
    WarzoneCard::Hand hand;
    std::cout << "Initial hand: " << hand << std::endl;
    
    OrdersList orders;
    Player alice("Alice");
    // Note: Player constructor should initialize OrdersList internally
    
    std::cout << "\n=== Drawing cards into hand repeatedly ===" << std::endl;
    
    // Draw cards until we have a few different types or deck is empty
    for (int i = 0; i < 7 && !deck.getCards().empty(); ++i) {
        if (deck.drawToHand(&hand)) {
            WarzoneCard::Card* drawnCard = hand.getHandCards().back();
            std::cout << "Drew: " << *drawnCard << std::endl;
        } else {
            std::cout << "Failed to draw card (deck empty)" << std::endl;
            break;
        }
    }
    
    std::cout << "\nAfter drawing:" << std::endl;
    std::cout << "Hand: " << hand << std::endl;
    std::cout << "Deck: " << deck << std::endl;
    
    std::cout << "\n=== Playing all cards in hand ===" << std::endl;
    
    while (!hand.getHandCards().empty()) {
        WarzoneCard::Card* cardToPlay = hand.getHandCards().front();
        std::cout << "Playing: " << *cardToPlay << std::endl;
        hand.playCard(cardToPlay, &alice, &deck);
    }
    
    std::cout << "\nFinal state:" << std::endl;
    std::cout << "Hand: " << hand << std::endl;
    std::cout << "Deck: " << deck << std::endl;
    std::cout << "Player's Orders: ";
    if (alice.getOrdersList()) {
        std::cout << *alice.getOrdersList() << std::endl;
    } else {
        std::cout << "No orders list" << std::endl;
    }
    
    std::cout << "\n=== Cards Module Test Complete ===" << std::endl;
}

// Main function for standalone testing
#ifndef MAIN_DRIVER_INCLUDED
int main() {
    testCards();
    return 0;
}
#endif
