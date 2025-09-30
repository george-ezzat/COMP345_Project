#pragma once
#ifndef GAMEENGINE_H
#define GAMEENGINE_H

#include <string>
#include <vector>
#include <iostream>


class Observer;

class GameEngine {
private:
    std::string* states;
    std::string* transitions;
    int* currentState;
    std::vector<Observer*> observers;

public:
    // Constructors/Destructor
    GameEngine();
    GameEngine(const GameEngine& other); // Copy constructor
    ~GameEngine();

    // methods
    bool validateCommand(const std::string& command) const;
    bool executeCommand(const std::string& command);
    std::string getCurrentState() const;
    void printCurrentState() const;

    // operator overloads (since they are objects as spoken in class)
    GameEngine& operator=(const GameEngine& other);
    friend std::ostream& operator<<(std::ostream& os, const GameEngine& engine);

    void addObserver(Observer* observer);
    void removeObserver(Observer* observer);
    void notify();
    std::string stringToLog() const;
};

#endif 