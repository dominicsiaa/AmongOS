#include "ConsoleManager.h"
#include "MainConsole.h"
#include <iostream>
#include <regex>

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

  
    // Regular expressions for screen -r <name> and screen -s <name>
    std::regex screenCommandR("screen -r (\\w+)");
    std::regex screenCommandS("screen -s (\\w+)");
    std::smatch match;

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
    //Handle `screen -s <name>`
    else if (std::regex_search(command, match, screenCommandS))
    {
        String processName = match[1].str();
        for (const auto& process : this->processTable)
        {
            if (process->getName() == processName)
            {
                std::cout << "Process with this name already exists!\n";
                return;
            }
        }
        auto process = std::make_shared<Process>(processName, 100);  
        this->addProcess(process);
        auto newScreen = std::make_shared<BaseScreen>(process, processName);

        ConsoleManager::getInstance()->registerScreen(newScreen);
        ConsoleManager::getInstance()->switchToScreen(processName);
    }
    // Handle `screen -r <name>`
    else if (std::regex_search(command, match, screenCommandR))
    {
        String processName = match[1].str();
        std::cout << "Retrieving process: " << processName << std::endl;

        for (const auto& process : this->processTable)
        {
            if (process->getName() == processName)
            {
                auto newScreen = std::make_shared<BaseScreen>(process, processName);
                ConsoleManager::getInstance()->registerScreen(newScreen);
                ConsoleManager::getInstance()->switchToScreen(processName);
                return;
            }
        }
        std::cerr << "Process '" <<processName <<"' not found\n";

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
	for (auto& process : this->processTable)
	{
		std::cout << "Process Name: " << process->getName() << std::endl;
		std::cout << "Process Current Line of Instruction: " << process->getCurrentLine() << std::endl;
		std::cout << "Process Total Lines of Instruction: " << process->getTotalLines() << std::endl;
	}
}










