#include "ConsoleManager.h"
#include "MainConsole.h"
#include <iostream>

MainConsole::MainConsole() : AConsole("MainConsole")
{
	
}

void MainConsole::onEnabled()
{
	std::cout << R"(  
   ____    ___ ___   ___   ____    ____       ___   _____
 _/__  |_ |   |   | /   \ |    \  /    |     /   \ / ___/
|____| | || _   _ ||     ||  _  ||   __|    |     (   \_ 
 |     | ||  \_/  ||  O  ||  |  ||  |  |    |  O  |\__  |
 |  _  | ||   |   ||     ||  |  ||  |_ |    |     |/  \ |
 |  |  |_||   |   ||     ||  |  ||     |    |     |\    |
 |__|__|  |___|___| \___/ |__|__||___,_|     \___/  \___|

)";

	std::cout << "\033[1;32m" << "Hello, Welcome to Among OS commandline!\n";
	std::cout << "\033[1;33m" << "Type 'exit' to quit, 'clear' to clear the screen\n";
}

void MainConsole::process()
{
    std::cout << "\033[0m" << "Enter a command: ";

	String command;
	std::getline(std::cin, command);

    if (command == "initialize") {
        std::cout << "\033[1;32m" << command + " command recognized. Doing Something\n";
    }
    else if (command == "screen") {
        std::cout << "\033[1;32m" << command + " command recognized. Doing Something\n";
    }
    else if (command == "scheduler-test") {
        std::cout << "\033[1;32m" << command + " command recognized. Doing Something\n";
    }
    else if (command == "scheduler-stop") {
        std::cout << "\033[1;32m" << command + " command recognized. Doing Something\n";
    }
    else if (command == "report-util") {
        std::cout << "\033[1;32m" << command + " command recognized. Doing Something\n";
    }
    else if (command == "clear") {
        system("cls");
        this->onEnabled();
    }
    else if (command == "exit") {
        std::cout << "\033[1;32m" << command + " command recognized. Doing Something\n";
        ConsoleManager::getInstance()->exitApplication();
    }
    //TODO: REMOVE WHEN DONE!!! This is just for testing, it moves to a test screen and you can exit from it woahhhhh :o
    else if (command == "test")
    {
        auto process = std::make_shared<Process>("ExampleProcess", 100);
        auto baseScreen = std::make_shared<BaseScreen>(process, "ExampleScreen");

        ConsoleManager::getInstance()->registerScreen(baseScreen);
        ConsoleManager::getInstance()->switchToScreen("ExampleScreen");
    }
    else {
        std::cout << "\033[1;31m" << "Error: command not recognized. Please try again\n";
    }
}

void MainConsole::display()
{
	
}

void MainConsole::addProcess(std::shared_ptr<Process> newProcess)
{
	this->processTable.push_back(newProcess);
}

void MainConsole::listProcesses() const
{
	
}










