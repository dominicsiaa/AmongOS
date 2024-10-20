#include "ConsoleManager.h"
#include "MainConsole.h"
#include <iostream>
#include <fstream>
#include <regex>
#include <sstream>
#include <chrono>
#include <ctime>

MainConsole::MainConsole() : AConsole("MainConsole") {
    this->isInitialized = false;
}

std::string displayHistory;

void MainConsole::appendToDisplayHistory(const std::string text) {
    displayHistory += text + "\n";
}

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
    std::cout << displayHistory;
}

void MainConsole::process() {
    std::cout << "\033[0m" << "Enter a command: ";

    String command;
    std::getline(std::cin, command);

    std::regex screenCommandR("screen -r (\\w+)");
    std::regex screenCommandS("screen -s (\\w+)");
    std::smatch match;
	
	appendToDisplayHistory("\033[1;37mEnter a command: " + command);

    // Handle `exit` command
    if (command == "exit") {
        std::cout << "\033[1;32mExiting application...\n";
        appendToDisplayHistory("\033[1;32mExiting application...");

        ConsoleManager::getInstance()->exitApplication();
        this->schedulerWorker.update(false);

    	return;
    }

    // Handle `initialize` command
    if (this->isInitialized == false)
    {
        if (command == "initialize") {
            try
            {
                std::ifstream configFile("config.txt");
                if (!configFile.is_open()) {
                    std::cerr << "\033[1;31mError: Could not open config.txt\n";
                    appendToDisplayHistory("\033[1;31mError: Could not open config.txt");
                    return;
                }

                std::unordered_map<String, String> configMap;
                String key;
                String value;

                while (configFile >> key >> value) {
                    configMap[key] = value;
                }

                configFile.close();

                this->config.num_cpu = std::stoul(configMap["num-cpu"]);
                if (this->config.num_cpu < 1 || this->config.num_cpu > 128) {
                    throw std::out_of_range("num-cpu must be between 1 and 128");
                }

                this->config.scheduler = configMap["scheduler"];
                if (this->config.scheduler != "\"fcfs\"" && this->config.scheduler != "\"rr\"") {
                    throw std::invalid_argument("Scheduler must be 'fcfs' or 'rr'");
                }

                this->config.quantum_cycles = std::stoul(configMap["quantum-cycles"]);
                this->config.batch_process_freq = std::stoul(configMap["batch-process-freq"]);
                this->config.min_ins = std::stoul(configMap["min-ins"]);
                this->config.max_ins = std::stoul(configMap["max-ins"]);
                this->config.delays_per_exec = std::stoul(configMap["delays-per-exec"]);

                FCFSScheduler::initialize(this->config.num_cpu);
                this->schedulerWorker.IThread::start();
                this->schedulerWorker.update(true);

                this->isInitialized = true;
                std::cout << "\033[1;32mSuccessfully initialized AmongOS\n";

                //std::cout << "\033[1;32m" << "Number of CPUs: " << this->config.num_cpu << "\n";
                //std::cout << "\033[1;32m" << "Scheduler: " << this->config.scheduler << "\n";
                //std::cout << "\033[1;32m" << "Quantum Cycles: " << this->config.quantum_cycles << "\n";
                //std::cout << "\033[1;32m" << "Batch Process Frequency: " << this->config.batch_process_freq << "\n";
                //std::cout << "\033[1;32m" << "Minimum Instructions: " << this->config.min_ins << "\n";
                //std::cout << "\033[1;32m" << "Maximum Instructions: " << this->config.max_ins << "\n";
                //std::cout << "\033[1;32m" << "Delays per Execution: " << this->config.delays_per_exec << "\n";

            	appendToDisplayHistory("\033[1;32mSuccessfully initialized AmongOS");
            }
            catch (const std::exception& e) {
				std::cerr << "\033[1;31mError: " << e.what() << "\n";
				appendToDisplayHistory("\033[1;31mError: " + std::string(e.what()));
			}
        }
        else {
			std::cout << "\033[1;31m" << "Error: System not initialized. Please run 'initialize' command\n";
			appendToDisplayHistory("\033[1;31mError: System not initialized. Please run 'initialize' command");
		}
        return;
    }

    // Handle all other commands
    if (command == "initialize") {
		std::cout << "\033[1;32m" << "System already initialized\n";
		appendToDisplayHistory("\033[1;32mSystem already initialized");
	}
    else if (command == "screen") {
        std::cout << "\033[1;32m" << command + " command recognized. Doing Something\n";
        appendToDisplayHistory("\033[1;32m" + command + " command recognized. Doing Something");
    }
    else if (command == "scheduler-test") {
        std::cout << "\033[1;32m" << command + " command recognized. Doing Something\n";
        appendToDisplayHistory("\033[1;32m" + command + " command recognized. Doing Something");
    }
    else if (command == "scheduler-stop") {
        std::cout << "\033[1;32m" << command + " command recognized. Doing Something\n";
        appendToDisplayHistory("\033[1;32m" + command + " command recognized. Doing Something");
    }
    else if (command == "report-util") {
        std::cout << "\033[1;32m" << command + " command recognized. Doing Something\n";
        appendToDisplayHistory("\033[1;32m" + command + " command recognized. Doing Something");
    }
    else if (command == "clear") {
        system("cls");
		displayHistory.clear();
        this->onEnabled();
    }

    // Handle `screen -s <name>`
    else if (std::regex_search(command, match, screenCommandS)) {
        String processName = match[1].str();
        for (const auto& process : this->processTable) {
            if (process->getName() == processName) {
                std::cout << "Process with this name already exists!\n";
				appendToDisplayHistory("Process with this name already exists!");
                return;
            }
        }
        std::stringstream timeStamp = createCurrentTimestamp();
        Process::RequirementFlags flags;
        flags.requireFiles = false;
        flags.numFiles = 0;
		flags.requireMemory = true;
        flags.memoryRequired = 1000;
        auto process = std::make_shared<Process>(0, processName, flags);
        this->addProcess(process);
        auto newScreen = std::make_shared<BaseScreen>(process, processName);

        ConsoleManager::getInstance()->registerScreen(newScreen);
        ConsoleManager::getInstance()->switchToScreen(processName);
    }
    // Handle `screen -r <name>`
    else if (std::regex_search(command, match, screenCommandR)) {
        String processName = match[1].str();
        std::cout << "Retrieving process: " << processName << std::endl;
		appendToDisplayHistory("Retrieving process: " + processName);

        for (const auto& process : this->processTable) {
            if (process->getName() == processName) {
                auto newScreen = std::make_shared<BaseScreen>(process, processName);
                ConsoleManager::getInstance()->registerScreen(newScreen);
                ConsoleManager::getInstance()->switchToScreen(processName);
                return;
            }
        }
        std::cerr << "Process '" << processName << "' not found\n";
        appendToDisplayHistory("Process '" + processName + "' not found");
    }
    // Handle `screen -ls`
    else if (command == "screen -ls") {
		FCFSScheduler::getInstance()->callScreenLS();
    }
    else {
        std::cout << "\033[1;31m" << "Error: command not recognized. Please try again\n";
		appendToDisplayHistory("\033[1;31mError: command not recognized. Please try again");
    }
}

void MainConsole::display() {}

void MainConsole::addProcess(std::shared_ptr<Process> newProcess) {
    this->processTable.push_back(newProcess);
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