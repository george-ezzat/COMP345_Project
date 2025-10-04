#include "GameEngine.h"
#include <iostream>
#include <string>

// Test function demonstrating GameEngine state transitions
void testGameStates() {
    std::cout << "=== Testing Game Engine States ===" << std::endl;

    GameEngine* engine = new GameEngine(); 
    std::string command;
    bool continueGame = true;

    std::cout << "Welcome to the Warzone Game Engine Test!" << std::endl;
    std::cout << "Enter commands to navigate through game states." << std::endl;
    std::cout << "Type 'exit' to quit the test." << std::endl;

    while (continueGame) {
        engine->printCurrentState();
        std::cout << "Enter command: ";
        std::getline(std::cin, command);

        if (command == "end") {
            std::cout << "Exiting game engine test." << std::endl;
            continueGame = false;
        }
        else {
            engine->executeCommand(command);
        }

        std::cout << std::endl;
    }
    
    delete engine;
}

#ifndef MAIN_DRIVER_INCLUDED
int main() {
    testGameStates();
    return 0;
}
#endif