#include "Cards.h"
#include <iostream>


#ifdef WZ_CARDS_STUB_DEPENDENCIES
namespace WarzoneOrder {
    class Order { 
    public: 
        virtual ~Order() = default; 
    };
    
    class OrdersList {
        std::vector<Order*> orders_;
    public:
        void addOrder(Order* o) { 
            if (o) orders_.push_back(o); 
        }
        friend std::ostream& operator<<(std::ostream& os, const OrdersList& ol) {
            return os << "OrdersList(size=" << ol.orders_.size() << ")";
        }
    };
    
    class Bomb : public Order {};
    class Reinforcement : public Order {};
    class Blockade : public Order {};
    class Airlift : public Order {};
    class Negotiate : public Order {};
}

namespace WarzonePlayer {
    class Player {
        WarzoneOrder::OrdersList* orders_{};
    public:
        void setPlayerOrders(WarzoneOrder::OrdersList* o) { 
            orders_ = o; 
        }
        WarzoneOrder::OrdersList* getPlayerOrders() const { 
            return orders_; 
        }
        void setHand(void*) {} 
    };
}
#endif

void testCards() {
    std::cout << "=== Testing Cards Module ===" << std::endl;
    
    WarzoneCard::Deck deck;
    std::cout << "Initial deck: " << deck << std::endl;
    
    WarzoneCard::Hand hand;
    std::cout << "Initial hand: " << hand << std::endl;
    
    WarzoneOrder::OrdersList orders;
    WarzonePlayer::Player alice;
    alice.setPlayerOrders(&orders);
    
    std::cout << "\n=== Drawing 5 cards into hand ===" << std::endl;
    
    for (int i = 0; i < 5; ++i) {
        WarzoneCard::Card* drawnCard = deck.draw();
        if (drawnCard) {
            std::cout << "Drew: " << *drawnCard << std::endl;
            hand.addCardToHand(drawnCard);
        } else {
            std::cout << "Failed to draw card (deck empty)" << std::endl;
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
    std::cout << "Orders: " << orders << std::endl;
    
    std::cout << "\n=== Cards Module Test Complete ===" << std::endl;
}

#ifdef WZ_CARDS_STUB_DEPENDENCIES
int main() {
    testCards();
    return 0;
}
#endif
