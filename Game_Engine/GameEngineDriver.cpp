#include "GameEngine.h"
#include <iostream>
#include <string>

// main driver function for the Game Engine component
void testGameStates() {
    std::cout << "=== Testing Game Engine States ===" << std::endl;

    // FIX 1: Change to pointer and use 'new'
    GameEngine* engine = new GameEngine(); 

    // For MainDriver: automated test with predefined commands
    #ifdef MAIN_DRIVER_INCLUDED
    std::cout << "Running automated GameEngine state transition demo:" << std::endl;
    
    // Show initial state
    engine->printCurrentState();
    
    // Demo different commands and state transitions
    std::string testCommands[] = {"loadmap Map/Asia.map", "validatemap", "addplayer Player1", "addplayer Player2", "assigncountries", "issueorder", "endissueorders", "execorder", "win", "end"};
    
    for (const std::string& cmd : testCommands) {
        std::cout << "\nExecuting command: " << cmd << std::endl;
        engine->executeCommand(cmd);
        engine->printCurrentState();
    }
    
    std::cout << "\nGameEngine automated test complete." << std::endl;
    
    #else
    // Interactive version for standalone testing
    std::string command;
    bool continueGame = true;

    std::cout << "Welcome to the Warzone Game Engine Test!" << std::endl;
    std::cout << "Enter commands to navigate through game states." << std::endl;
    std::cout << "Type 'exit' to quit the test." << std::endl;

    while (continueGame) {

         // FIX 2: Use pointer access '->'
        engine->printCurrentState();
        std::cout << "Enter command: ";
        std::getline(std::cin, command);


        if (command == "exit") {
            std::cout << "Exiting game engine test." << std::endl;
            continueGame = false;
        }
        // Execute the command
        else {
            // FIX 3: Use pointer access '->'
            engine->executeCommand(command);
        }

        std::cout << std::endl;
    }
    #endif
    
    // FIX 4: Clean up memory before exiting test function
    delete engine;
}

// For testing 
#ifndef MAIN_DRIVER_INCLUDED
int main() {
    testGameStates();
    return 0;
}
#endif