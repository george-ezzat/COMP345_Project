#include "Cards.h"
#include "../Player/Player.h"
#include "../Orders/Orders.h"
#include <algorithm>
#include <stdexcept>

namespace WarzoneCard {


    Card::Card() : type(CardType::Unknown) {}

    Card::Card(CardType t) : type(t) {}

    Card::Card(const Card& other) : type(other.type) {}

    Card& Card::operator=(const Card& other) {
        if (this != &other) {
            type = other.type;
        }
        return *this;
    }

    Card::~Card() {
    }

    CardType Card::getType() const {
        return type;
    }

    void Card::setType(CardType t) {
        type = t;
    }

    void Card::play(Player* player) {
        if (!player) {
            return;
        }

        Order* order = nullptr;
        std::cout << "Card::play() called for ";
        
        switch (type) {
            case CardType::Bomb:
                std::cout << "Bomb card - creating Bomb order";
                order = new Bomb();
                break;
            case CardType::Reinforcement:
                std::cout << "Reinforcement card - creating Deploy order (reinforcement equivalent)";
                order = new Deploy();
                break;
            case CardType::Blockade:
                std::cout << "Blockade card - creating Blockade order";
                order = new Blockade();
                break;
            case CardType::Airlift:
                std::cout << "Airlift card - creating Airlift order";
                order = new Airlift();
                break;
            case CardType::Diplomacy:
                std::cout << "Diplomacy card - creating Negotiate order";
                order = new Negotiate();
                break;
            case CardType::Unknown:
            default:
                std::cout << "Unknown card - no action taken";
                return;
        }
        
        if (order && player->getOrdersList()) {
            player->getOrdersList()->add(order);
            std::cout << " - order added to player's order list";
        }
        std::cout << std::endl;
    }

    std::ostream& operator<<(std::ostream& os, const Card& card) {
        switch (card.type) {
            case CardType::Bomb:
                os << "Card(Bomb)";
                break;
            case CardType::Reinforcement:
                os << "Card(Reinforcement)";
                break;
            case CardType::Blockade:
                os << "Card(Blockade)";
                break;
            case CardType::Airlift:
                os << "Card(Airlift)";
                break;
            case CardType::Diplomacy:
                os << "Card(Diplomacy)";
                break;
            case CardType::Unknown:
            default:
                os << "Card(Unknown)";
                break;
        }
        return os;
    }


    // Constructor - creates a deck with 25 cards (5 of each type)
    Deck::Deck() : cards(new std::vector<Card*>()), gen(rd()) {
        for (int i = 0; i < 5; ++i) {
            cards->push_back(new Card(CardType::Bomb));
            cards->push_back(new Card(CardType::Reinforcement));
            cards->push_back(new Card(CardType::Blockade));
            cards->push_back(new Card(CardType::Airlift));
            cards->push_back(new Card(CardType::Diplomacy));
        }
    }

    Deck::Deck(const Deck& other) : cards(new std::vector<Card*>()), gen(rd()) {
        for (const Card* card : *other.cards) {
            if (card) {
                cards->push_back(new Card(*card));
            }
        }
    }

    Deck& Deck::operator=(const Deck& other) {
        if (this != &other) {
            std::vector<Card*>* newCards = new std::vector<Card*>();
            
            try {
                for (const Card* card : *other.cards) {
                    if (card) {
                        newCards->push_back(new Card(*card));
                    }
                }
                
                for (Card* card : *cards) {
                    delete card;
                }
                delete cards;
                
                cards = newCards;
            } catch (...) {
                for (Card* card : *newCards) {
                    delete card;
                }
                delete newCards;
                throw;
            }
        }
        return *this;
    }

    Deck::~Deck() {
        for (Card* card : *cards) {
            delete card;
        }
        delete cards;
    }

    // Get reference to the cards vector
    const std::vector<Card*>& Deck::getCards() const {
        return *cards;
    }

    // Replace all cards in deck with new set of cards
    void Deck::setCards(const std::vector<Card*>& newCards) {
        for (Card* card : *cards) {
            delete card;
        }
        cards->clear();
        
        for (const Card* card : newCards) {
            if (card) {
                cards->push_back(new Card(*card));
            }
        }
    }

    // Draw a random card from the deck
    Card* Deck::draw() {
        if (cards->empty()) {
            return nullptr;
        }
        
        std::uniform_int_distribution<size_t> dist(0, cards->size() - 1);
        size_t index = dist(gen);
        
        Card* drawnCard = (*cards)[index];
        cards->erase(cards->begin() + index);
        
        return drawnCard;
    }

    // Draw a card and add it directly to the specified hand
    bool Deck::drawToHand(Hand* hand) {
        if (!hand) {
            return false;
        }
        
        Card* drawnCard = draw();
        if (drawnCard) {
            hand->addCardToHand(drawnCard);
            return true;
        }
        return false;
    }

    // Return a played card back to the deck
    void Deck::returnToDeck(Card* card) {
        if (card) {
            cards->push_back(card);
        }
    }

    std::ostream& operator<<(std::ostream& os, const Deck& deck) {
        os << "Deck contains " << deck.cards->size() << " cards: [";
        for (size_t i = 0; i < deck.cards->size(); ++i) {
            if (i > 0) os << ", ";
            os << *(*deck.cards)[i];
        }
        os << "]";
        return os;
    }

    // Constructor - creates an empty hand
    Hand::Hand() : handCards(new std::vector<Card*>()) {}

    // Copy constructor
    Hand::Hand(const Hand& other) : handCards(new std::vector<Card*>()) {
        for (Card* card : *other.handCards) {
            handCards->push_back(card);
        }
    }

    Hand& Hand::operator=(const Hand& other) {
        if (this != &other) {
            handCards->clear();
            for (Card* card : *other.handCards) {
                handCards->push_back(card);
            }
        }
        return *this;
    }

    Hand::~Hand() {
        delete handCards;
    }

    // Get reference to the hand's cards
    const std::vector<Card*>& Hand::getHandCards() const {
        return *handCards;
    }

    // Replace all cards in hand with new set
    void Hand::setHandCards(const std::vector<Card*>& src) {
        handCards->clear();
        for (Card* card : src) {
            handCards->push_back(card);
        }
    }

    // Add a card to the hand
    void Hand::addCardToHand(Card* c) {
        if (c) {
            handCards->push_back(c);
        }
    }

    // Remove a specific card from the hand
    void Hand::removeCardFromHand(Card* c) {
        if (c) {
            auto it = std::find(handCards->begin(), handCards->end(), c);
            if (it != handCards->end()) {
                handCards->erase(it);
            }
        }
    }

    // Play a card - executes its effect and returns it to deck
    void Hand::playCard(Card* c, Player* player, Deck* deck) {
        if (!c || !player || !deck) {
            return;
        }
        
        c->play(player);
        
        removeCardFromHand(c);
        
        deck->returnToDeck(c);
    }

    std::ostream& operator<<(std::ostream& os, const Hand& hand) {
        os << "Hand contains " << hand.handCards->size() << " cards: [";
        for (size_t i = 0; i < hand.handCards->size(); ++i) {
            if (i > 0) os << ", ";
            os << *(*hand.handCards)[i];
        }
        os << "]";
        return os;
    }

} 
