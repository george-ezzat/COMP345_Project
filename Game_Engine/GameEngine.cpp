#include "GameEngine.h"
#include <iostream>
#include <algorithm>
#include <sstream>

// NEW: Add includes for the component headers
// NOTE: Adjust paths if your GameEngine.cpp is not at the same level as the folders
#include "../Map/Map.h" 
#include "../Cards/Cards.h"  // Contains WarzoneCard::Deck
#include "../Player/Player.h" 

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

    // NEW: Initialize all component pointers
    gameMap = nullptr; // Map is loaded later, starts null
    gameDeck = new WarzoneCard::Deck(); // Deck is created at start
    players = new std::vector<Player*>(); // The vector itself is a pointer
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
    // NEW: Clean up the game components
    delete gameDeck;
    gameDeck = nullptr;

    // Clean up all Player objects in the vector
    for (Player* p : *players) {
        delete p;
    }
    players->clear();
    // Then clean up the vector pointer itself
    delete players;
    players = nullptr;

    // Clean up map only if it was loaded
    if (gameMap != nullptr) {
        delete gameMap;
        gameMap = nullptr;
    }
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
        // 1. loadmap <filename>
        if (command.rfind("loadmap", 0) == 0) {
            
            // Extract the filename from the command string
            std::stringstream ss(command);
            std::string cmd;
            std::string filename;
            ss >> cmd >> filename; 
            
            // *** INTEGRATION POINT 1: MapLoader ***
            MapLoader loader; 
            Map* loadedMap = loader.loadMap(filename);

            if (loadedMap) {
                if (gameMap != nullptr) delete gameMap; 
                gameMap = loadedMap;
                std::cout << "Map " << filename << " loaded successfully." << std::endl;
                *currentState = 1; // move to "map loaded" state
                notify();
                return true;
            } else {
                std::cout << "Error: Failed to load map " << filename << ". Staying in 'start' state." << std::endl;
                return false;
            }
        }
    }
    else if (*currentState == 1) { // map loaded
        // 2. validatemap
        if (command == "validatemap") {
            // *** INTEGRATION POINT 2: Map Validation ***
            if (gameMap != nullptr && gameMap->validate()) { 
                std::cout << "Map successfully validated." << std::endl;
                *currentState = 2; // move to "map validated" state
                notify();
                return true;
            } else {
                std::cout << "Map validation FAILED. Staying in 'map loaded' state." << std::endl;
                return false;
            }
        }
        }
        else if (command == "validatemap") {
            *currentState = 2; // move to "map validated"
            notify();
            return true;
        }
    //}
    else if (*currentState == 2) { // map validated
        // 3. addplayer <name>
        if (command.rfind("addplayer", 0) == 0) {
            
            std::stringstream ss(command);
            std::string cmd;
            std::string name;
            ss >> cmd >> name;

            if (players->size() >= 6) {
                std::cout << "Cannot add player. Maximum of 6 players reached." << std::endl;
                return false;
            }

            // *** INTEGRATION POINT 3: Player Creation ***
            Player* newPlayer = new Player(name); 
            players->push_back(newPlayer);
            std::cout << "Player " << name << " added. Total players: " << players->size() << std::endl;
            
            // State remains "map validated" to allow adding more players
            return true;
            }
        
        // 4. gamestart (to start the main assignment phase)
        if (command == "gamestart") {
            if (players->size() >= 2) {
                std::cout << "Game started. Moving to 'players added' state to begin setup." << std::endl;
                // NOTE: The logic for assigncountries will happen between states 2 and 3 in the final version.
                *currentState = 3; // move to "players added" state
                notify();
                return true;
            } else {
                std::cout << "Cannot start game. Need at least 2 players." << std::endl;
                return false;
            }
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