#include "GameEngine.h"
#include <iostream>
#include <string>

// main driver function for the Game Engine component
void testGameStates() {
    std::cout << "=== Testing Game Engine States ===" << std::endl;


    GameEngine engine;

    std::string command;
    bool continueGame = true;

    std::cout << "Welcome to the Warzone Game Engine Test!" << std::endl;
    std::cout << "Enter commands to navigate through game states." << std::endl;
    std::cout << "Type 'exit' to quit the test." << std::endl;

    while (continueGame) {

        engine.printCurrentState();
        std::cout << "Enter command: ";
        std::getline(std::cin, command);


        if (command == "exit") {
            std::cout << "Exiting game engine test." << std::endl;
            continueGame = false;
        }
        // Execute the command
        else {
            engine.executeCommand(command);
        }

        std::cout << std::endl;
    }
}

// For testing 
int main() {
    testGameStates();
    return 0;
}