#include "ConsoleManager.h"
#include "MainConsole.h"
#include <iostream>
#include <regex>
#include <sstream>
#include <chrono>
#include <ctime>

MainConsole::MainConsole() : AConsole("MainConsole") {}

void MainConsole::onEnabled() {
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

void MainConsole::process() {
    std::cout << "\033[0m" << "Enter a command: ";

    String command;
    std::getline(std::cin, command);

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
    // Handle `screen -s <name>`
    else if (std::regex_search(command, match, screenCommandS)) {
        String processName = match[1].str();
        for (const auto& process : this->processTable) {
            if (process->getName() == processName) {
                std::cout << "Process with this name already exists!\n";
                return;
            }
        }
        std::stringstream timeStamp = createCurrentTimestamp();
        auto process = std::make_shared<Process>(processName, 100, std::move(timeStamp));
        this->addProcess(process);
        auto newScreen = std::make_shared<BaseScreen>(process, processName);

        ConsoleManager::getInstance()->registerScreen(newScreen);
        ConsoleManager::getInstance()->switchToScreen(processName);
    }
    // Handle `screen -r <name>`
    else if (std::regex_search(command, match, screenCommandR)) {
        String processName = match[1].str();
        std::cout << "Retrieving process: " << processName << std::endl;

        for (const auto& process : this->processTable) {
            if (process->getName() == processName) {
                auto newScreen = std::make_shared<BaseScreen>(process, processName);
                ConsoleManager::getInstance()->registerScreen(newScreen);
                ConsoleManager::getInstance()->switchToScreen(processName);
                return;
            }
        }
        std::cerr << "Process '" << processName << "' not found\n";
    }
    // Handle `screen -ls`
    else if (command == "screen -ls") {
        listProcesses();
    }
    else {
        std::cout << "\033[1;31m" << "Error: command not recognized. Please try again\n";
    }
}

void MainConsole::display() {}

void MainConsole::addProcess(std::shared_ptr<Process> newProcess) {
    this->processTable.push_back(newProcess);
}

void MainConsole::listProcesses() const {
    std::cout << "CPU utilization: 100%\n";
    std::cout << "Cores used: 4\n";
    std::cout << "Cores available: 0\n";
    std::cout << "\n-------------------------------------------------------------\n";
    std::cout << "Running Processes:\n";
    for (const auto& process : this->processTable) {
        std::cout << "Process Name:\t" << process->getName()
            << "\t(" << process->getTimeStamp().str() << ")\tCore: 0\t"
            << process->getCurrentLine() << "/" << process->getTotalLines()
            << std::endl;
    }
}

std::stringstream MainConsole::createCurrentTimestamp() const {
    auto now = std::chrono::system_clock::now();
    auto nowTime = std::chrono::system_clock::to_time_t(now);

    std::tm local_tm;
    localtime_s(&local_tm, &nowTime);

    std::stringstream timeStream;
    timeStream << std::put_time(&local_tm, "%m/%d/%Y %I:%M:%S %p");
    return timeStream;
}