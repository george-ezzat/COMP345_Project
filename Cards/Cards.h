#pragma once

#include <vector>
#include <iostream>
#include <random>

namespace WarzonePlayer { class Player; }
namespace WarzoneOrder { 
    class Order; 
    class OrdersList; 
    class Bomb;
    class Reinforcement;
    class Blockade;
    class Airlift;
    class Negotiate;
}

namespace WarzoneCard {

    enum class CardType { 
        Unknown, 
        Bomb, 
        Reinforcement, 
        Blockade, 
        Airlift, 
        Diplomacy 
    };

    class Card {
    private:
        CardType type;

    public:
        Card();
        Card(CardType t);
        
        Card(const Card& other);
        Card& operator=(const Card& other);
        ~Card();
        
        CardType getType() const;
        void setType(CardType t);
        
        void play(WarzonePlayer::Player* player);
        
        friend std::ostream& operator<<(std::ostream& os, const Card& card);
    };

    class Deck {
    private:
        std::vector<Card*>* cards;
        std::random_device rd;
        std::mt19937 gen;

    public:
        Deck();
        
        Deck(const Deck& other);
        Deck& operator=(const Deck& other);
        ~Deck();
        
        const std::vector<Card*>& getCards() const;
        void setCards(const std::vector<Card*>& newCards);
        
        Card* draw();
        void returnToDeck(Card* card);
        
        friend std::ostream& operator<<(std::ostream& os, const Deck& deck);
    };

    class Hand {
    private:
        std::vector<Card*>* handCards;

    public:
        Hand();
        
        Hand(const Hand& other);
        Hand& operator=(const Hand& other);
        ~Hand();
        
        const std::vector<Card*>& getHandCards() const;
        void setHandCards(const std::vector<Card*>& src);
        
        void addCardToHand(Card* c);
        void removeCardFromHand(Card* c);
        void playCard(Card* c, WarzonePlayer::Player* player, Deck* deck);
        
        friend std::ostream& operator<<(std::ostream& os, const Hand& hand);
    };

} 
