#include "GameEngine.h"
#include <iostream>
#include <algorithm>

// Constructor
GameEngine::GameEngine() {
    // game states
    states = new std::string[8]{
        "start", "map loaded", "map validated", "players added",
        "assign reinforcement", "issue orders", "execute orders", "win"
    };

    // transitions
    transitions = new std::string[11]{
        "loadmap", "validatemap", "addplayer", "assigncountries",
        "issueorder", "endissueorders", "execorder", "endexecorders",
        "win", "play", "end"
    };

    // start at the initial state
    currentState = new int(0);
}

// Copy constructor
GameEngine::GameEngine(const GameEngine& other) {
    states = new std::string[8];
    transitions = new std::string[11];
    currentState = new int(*(other.currentState));

    for (int i = 0; i < 8; i++) {
        states[i] = other.states[i];
    }

    for (int i = 0; i < 11; i++) {
        transitions[i] = other.transitions[i];
    }

    observers = other.observers;
}

// Destructor
GameEngine::~GameEngine() {
    delete[] states;
    delete[] transitions;
    delete currentState;
}

// Assignment operator
GameEngine& GameEngine::operator=(const GameEngine& other) {
    if (this != &other) {
        delete[] states;
        delete[] transitions;
        delete currentState;

        states = new std::string[8];
        transitions = new std::string[11];
        currentState = new int(*(other.currentState));

        for (int i = 0; i < 8; i++) {
            states[i] = other.states[i];
        }

        for (int i = 0; i < 11; i++) {
            transitions[i] = other.transitions[i];
        }

        observers = other.observers;
    }
    return *this;
}

//  insertion operator
std::ostream& operator<<(std::ostream& os, const GameEngine& engine) {
    os << "Current Game State: " << engine.states[*(engine.currentState)];
    return os;
}

// commands for current state (same as the one in the assignment diagram)
bool GameEngine::validateCommand(const std::string& command) const {
    switch (*currentState) {
    case 0: // start
        return command == "loadmap";
    case 1: // map loaded
        return command == "loadmap" || command == "validatemap";
    case 2: // map validated
        return command == "addplayer";
    case 3: // players added
        return command == "addplayer" || command == "assigncountries";
    case 4: // assign reinforcement
        return command == "issueorder";
    case 5: // issue orders
        return command == "issueorder" || command == "endissueorders";
    case 6: // execute orders
        return command == "execorder" || command == "endexecorders" || command == "win";
    case 7: // win
        return command == "play" || command == "end";
    default:
        return false;
    }
}

// if command valid change/stay state
bool GameEngine::executeCommand(const std::string& command) {
    if (!validateCommand(command)) {
        std::cout << "Invalid command '" << command << "' for current state '"
            << states[*currentState] << "'" << std::endl;
        return false;
    }

    if (*currentState == 0) { // start
        if (command == "loadmap") {
            *currentState = 1; // move to "map loaded"
            notify();
            return true;
        }
    }
    else if (*currentState == 1) { // map loaded
        if (command == "loadmap") {
            // Stay in the same state
            notify();
            return true;
        }
        else if (command == "validatemap") {
            *currentState = 2; // move to "map validated"
            notify();
            return true;
        }
    }
    else if (*currentState == 2) { // map validated
        if (command == "addplayer") {
            *currentState = 3; // move to "players added"
            notify();
            return true;
        }
    }
    else if (*currentState == 3) { // players added
        if (command == "addplayer") {
            // Stay in the same state
            notify();
            return true;
        }
        else if (command == "assigncountries") {
            *currentState = 4; // move to "assign reinforcement"
            notify();
            return true;
        }
    }
    else if (*currentState == 4) { // assign reinforcement
        if (command == "issueorder") {
            *currentState = 5; // move to "issue orders"
            notify();
            return true;
        }
    }
    else if (*currentState == 5) { // issue orders
        if (command == "issueorder") {
            // Stay in the same state
            notify();
            return true;
        }
        else if (command == "endissueorders") {
            *currentState = 6; // move to "execute orders"
            notify();
            return true;
        }
    }
    else if (*currentState == 6) { // execute orders
        if (command == "execorder") {
            // Stay in the same state
            notify();
            return true;
        }
        else if (command == "endexecorders") {
            *currentState = 4; // go back to "assign reinforcement"
            notify();
            return true;
        }
        else if (command == "win") {
            *currentState = 7; // move to "win" state
            notify();
            return true;
        }
    }
    else if (*currentState == 7) { // win
        if (command == "play") {
            *currentState = 0; // go back to "start"
            notify();
            return true;
        }
        else if (command == "end") {
            // Game over
            std::cout << "Game ended" << std::endl;
            return true;
        }
    }

    return false;
}

// get state name
std::string GameEngine::getCurrentState() const {
    return states[*currentState];
}

// print current state
void GameEngine::printCurrentState() const {
    std::cout << "Current state: " << states[*currentState] << std::endl;
}

void GameEngine::addObserver(Observer* observer) {
    observers.push_back(observer);
}

void GameEngine::removeObserver(Observer* observer) {
    observers.erase(std::remove(observers.begin(), observers.end(), observer), observers.end());
}

void GameEngine::notify() {
    for (auto observer : observers) {
        // Assuming observer has an update method
        // observer->update(this);
    }
}

std::string GameEngine::stringToLog() const {
    return "GameEngine State Change: Current state is " + states[*currentState];
}