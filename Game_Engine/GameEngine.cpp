#include "GameEngine.h"
#include <iostream>
#include <algorithm>
#include <sstream>

#include "../Map/Map.h" 
#include "../Cards/Cards.h"
#include "../Player/Player.h" 

GameEngine::GameEngine() {
    states = new std::string[8]{
        "start", "map loaded", "map validated", "players added",
        "assign reinforcement", "issue orders", "execute orders", "win"
    };

    transitions = new std::string[11]{
        "loadmap", "validatemap", "addplayer", "assigncountries",
        "issueorder", "endissueorders", "execorder", "endexecorders",
        "win", "play", "end"
    };

    currentState = new int(0);

    gameMap = nullptr;
    gameDeck = new WarzoneCard::Deck();
    players = new std::vector<Player*>();
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

GameEngine::~GameEngine() {
    delete[] states;
    delete[] transitions;
    delete currentState;
    delete gameDeck;
    gameDeck = nullptr;

    for (Player* p : *players) {
        delete p;
    }
    players->clear();
    delete players;
    players = nullptr;

    if (gameMap != nullptr) {
        delete gameMap;
        gameMap = nullptr;
    }
}

std::ostream& operator<<(std::ostream& os, const GameEngine& engine) {
    os << "Current Game State: " << engine.states[*(engine.currentState)];
    return os;
}

bool GameEngine::validateCommand(const std::string& command) const {
    switch (*currentState) {
    case 0: // start
        return command.rfind("loadmap", 0) == 0; // Check if command starts with "loadmap"
    case 1: // map loaded
        return command.rfind("loadmap", 0) == 0 || command == "validatemap";
    case 2: // map validated
        return command.rfind("addplayer", 0) == 0; // Check if command starts with "addplayer"
    case 3: // players added
        return command.rfind("addplayer", 0) == 0 || command == "assigncountries";
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

bool GameEngine::executeCommand(const std::string& command) {
    if (!validateCommand(command)) {
        std::cout << "Invalid command '" << command << "' for current state '"
            << states[*currentState] << "'" << std::endl;
        return false;
    }

    if (*currentState == 0) {
        if (command.rfind("loadmap", 0) == 0) {
            std::stringstream ss(command);
            std::string cmd;
            std::string filename;
            ss >> cmd >> filename; 
            
            MapLoader loader; 
            Map* loadedMap = loader.loadMap(filename);

            if (loadedMap) {
                if (gameMap != nullptr) delete gameMap; 
                gameMap = loadedMap;
                std::cout << "Map " << filename << " loaded successfully." << std::endl;
                *currentState = 1;
                notify();
                return true;
            } else {
                std::cout << "Error: Failed to load map " << filename << ". Staying in 'start' state." << std::endl;
                return false;
            }
        }
    }
    else if (*currentState == 1) {
        if (command == "validatemap") {
            if (gameMap != nullptr && gameMap->validate()) { 
                std::cout << "Map successfully validated." << std::endl;
                *currentState = 2;
                notify();
                return true;
            } else {
                std::cout << "Map validation FAILED. Staying in 'map loaded' state." << std::endl;
                return false;
            }
        }
        else if (command == "validatemap") {
            *currentState = 2;
            notify();
            return true;
        }
    }
    else if (*currentState == 2) {
        if (command.rfind("addplayer", 0) == 0) {
            
            std::stringstream ss(command);
            std::string cmd;
            std::string name;
            ss >> cmd >> name;

            if (players->size() >= 6) {
                std::cout << "Cannot add player. Maximum of 6 players reached." << std::endl;
                return false;
            }

            Player* newPlayer = new Player(name); 
            players->push_back(newPlayer);
            std::cout << "Player " << name << " added. Total players: " << players->size() << std::endl;
            
            *currentState = 3;
            notify();
            return true;
        }
        
        if (command == "gamestart") {
            if (players->size() >= 2) {
                std::cout << "Game started. Moving to 'players added' state to begin setup." << std::endl;
                *currentState = 3;
                notify();
                return true;
            } else {
                std::cout << "Cannot start game. Need at least 2 players." << std::endl;
                return false;
            }
        }
    }
    else if (*currentState == 3) {
        if (command.rfind("addplayer", 0) == 0) {
            std::stringstream ss(command);
            std::string cmd;
            std::string name;
            ss >> cmd >> name;

            if (players->size() >= 6) {
                std::cout << "Cannot add player. Maximum of 6 players reached." << std::endl;
                return false;
            }

            Player* newPlayer = new Player(name); 
            players->push_back(newPlayer);
            std::cout << "Player " << name << " added. Total players: " << players->size() << std::endl;
            
            notify();
            return true;
        }
        else if (command == "assigncountries") {
            *currentState = 4;
            notify();
            return true;
        }
    }
    else if (*currentState == 4) {
        if (command == "issueorder") {
            *currentState = 5;
            notify();
            return true;
        }
    }
    else if (*currentState == 5) {
        if (command == "issueorder") {
            notify();
            return true;
        }
        else if (command == "endissueorders") {
            *currentState = 6;
            notify();
            return true;
        }
    }
    else if (*currentState == 6) {
        if (command == "execorder") {
            notify();
            return true;
        }
        else if (command == "endexecorders") {
            *currentState = 4;
            notify();
            return true;
        }
        else if (command == "win") {
            *currentState = 7;
            notify();
            return true;
        }
    }
    else if (*currentState == 7) {
        if (command == "play") {
            *currentState = 0;
            notify();
            return true;
        }
        else if (command == "end") {
            std::cout << "Game ended" << std::endl;
            return true;
        }
    }

    return false;
}

std::string GameEngine::getCurrentState() const {
    return states[*currentState];
}

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
    }
}

std::string GameEngine::stringToLog() const {
    return "GameEngine State Change: Current state is " + states[*currentState];
}