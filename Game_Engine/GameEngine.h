#pragma once
#ifndef GAMEENGINE_H
#define GAMEENGINE_H

#include <string>
#include <vector>
#include <iostream>


class Observer;

class Map;
class Player;
namespace WarzoneCard { class Deck; }

class GameEngine {
private:
    std::string* states;
    std::string* transitions;
    int* currentState;
    std::vector<Observer*> observers;

    Map* gameMap;
    WarzoneCard::Deck* gameDeck;
    std::vector<Player*>* players; 

public:
    GameEngine();
    GameEngine(const GameEngine& other);  // Copy constructor
    ~GameEngine();

    bool validateCommand(const std::string& command) const;
    bool executeCommand(const std::string& command);
    std::string getCurrentState() const;
    void printCurrentState() const;

    GameEngine& operator=(const GameEngine& other);  // Assignment operator
    friend std::ostream& operator<<(std::ostream& os, const GameEngine& engine);

    void addObserver(Observer* observer);
    void removeObserver(Observer* observer);
    void notify();
    std::string stringToLog() const;
};

#endif 