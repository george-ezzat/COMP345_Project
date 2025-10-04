#include "GameEngine.h"
#include <iostream>
#include <string>

// Test function demonstrating GameEngine state transitions
void testGameStates() {
    std::cout << "=== Testing Game Engine States ===" << std::endl;

    GameEngine* engine = new GameEngine(); 

    #ifdef MAIN_DRIVER_INCLUDED
    std::cout << "Running automated GameEngine state transition demo:" << std::endl;
    
    engine->printCurrentState();
    
    std::string testCommands[] = {"loadmap Map/Asia.map", "validatemap", "addplayer Player1", "addplayer Player2", "assigncountries", "issueorder", "endissueorders", "execorder", "win", "end"};
    
    for (const std::string& cmd : testCommands) {
        std::cout << "\nExecuting command: " << cmd << std::endl;
        engine->executeCommand(cmd);
        engine->printCurrentState();
    }
    
    std::cout << "\nGameEngine automated test complete." << std::endl;
    
    #else
    std::string command;
    bool continueGame = true;

    std::cout << "Welcome to the Warzone Game Engine Test!" << std::endl;
    std::cout << "Enter commands to navigate through game states." << std::endl;
    std::cout << "Type 'exit' to quit the test." << std::endl;

    while (continueGame) {
        engine->printCurrentState();
        std::cout << "Enter command: ";
        std::getline(std::cin, command);

        if (command == "exit") {
            std::cout << "Exiting game engine test." << std::endl;
            continueGame = false;
        }
        else {
            engine->executeCommand(command);
        }

        std::cout << std::endl;
    }
    #endif
    
    delete engine;
}

#ifndef MAIN_DRIVER_INCLUDED
int main() {
    testGameStates();
    return 0;
}
#endif