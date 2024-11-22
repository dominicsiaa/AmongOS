#include "ConsoleManager.h"
#include "MainConsole.h"
#include <iostream>
#include <fstream>
#include <regex>
#include <sstream>
#include <chrono>
#include <ctime>
#include <cstdlib>

MainConsole::MainConsole() : AConsole("MainConsole") {
    this->isInitialized = false;
    this->inputWorker = std::make_shared<InputWorker>();
}

std::string displayHistory;

bool MainConsole::isSchedulerTestRunning = false;

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
    std::cout << "\033[0m" << "Enter a command: ";

    this->command.clear();
    this->commandEntered = false;

    this->inputWorker->update(true);
}

void MainConsole::runSchedulerTest() {
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    static int counter = 1;

    if (counter == config.batch_process_freq) {
        counter = 1;
    }
    else {
        counter++;
        return;
    }

    std::stringstream timeStamp = createCurrentTimestamp();

    Process::RequirementFlags flags;
    flags.requireFiles = false;
    flags.numFiles = 0;
    flags.memoryRequired = config.min_mem_per_proc +
        (std::rand() % (config.max_mem_per_proc - config.min_mem_per_proc + 1));
    flags.requireMemory = true;

    auto process = std::make_shared<Process>(processCounter, "Process" + std::to_string(processCounter), flags);
    process->generateDummyCommands(config.min_ins, config.max_ins);
    this->addProcess(process);
    processCounter++;
}

void MainConsole::process() {

    //sleep for 1 ms
    //std::this_thread::sleep_for(std::chrono::milliseconds(1));
	//std::this_thread::sleep_for(std::chrono::milliseconds(1));
    std::this_thread::sleep_for(std::chrono::microseconds(50));

    if (MainConsole::isSchedulerTestRunning) {
        runSchedulerTest();
    }

    if(!this->commandEntered) {
		return;
	}

    std::regex screenCommandR("^screen -r ([\\w-]+)");
    std::regex screenCommandS("^screen -s ([\\w-]+)");
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
	else if (this->isInitialized == false)
    {
        if (command == "initialize") {
            try
            {
                std::ifstream configFile("config.txt");
                if (!configFile.is_open()) {
                    throw std::runtime_error("config.txt not found");
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
                    throw std::invalid_argument("scheduler must be 'fcfs' or 'rr'");
                }

                this->config.quantum_cycles = std::stoul(configMap["quantum-cycles"]);
                this->config.batch_process_freq = std::stoul(configMap["batch-process-freq"]);
                this->config.min_ins = std::stoul(configMap["min-ins"]);
                this->config.max_ins = std::stoul(configMap["max-ins"]);
                this->config.delay_per_exec = std::stoul(configMap["delay-per-exec"]);
                this->config.max_overall_mem = std::stoul(configMap["max-overall-mem"]);
                this->config.mem_per_frame = std::stoul(configMap["mem-per-frame"]);
                this->config.min_mem_per_proc = std::stoul(configMap["min-mem-per-proc"]);
				this->config.max_mem_per_proc = std::stoul(configMap["max-mem-per-proc"]);

                GlobalScheduler::initialize(this->config.num_cpu, this->config.quantum_cycles, this->config.scheduler, this->config.delay_per_exec, this->config.max_overall_mem);
                this->isInitialized = true;

            	std::cout << "\033[1;32mSuccessfully initialized AmongOS\n";

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
    }
    else if (command == "initialize") {
		std::cout << "\033[1;32m" << "System already initialized\n";
		appendToDisplayHistory("\033[1;32mSystem already initialized");
	}
    else if (command == "screen") {
        std::cout << "\033[1;33m" << "Available screen commands:\n";
        std::cout << "\033[1;33m" << "-ls : List all running and finished processes\n";
        std::cout << "\033[1;33m" << "-r <name> : Retrieve and display a single process by name\n";
        std::cout << "\033[1;33m" << "-s <name> : Create a new process with the specified name\n";

        appendToDisplayHistory("\033[1;33mAvailable screen commands:");
        appendToDisplayHistory("\033[1;33m-ls : List all running and finished processes");
        appendToDisplayHistory("\033[1;33m-r <name> : Retrieve and display a single process by name");
        appendToDisplayHistory("\033[1;33m-s <name> : Create a new process with the specified name");
    }
    else if (command == "scheduler-test") {
        if (!MainConsole::isSchedulerTestRunning) {
            std::cout << "\033[1;32mStarting scheduler test...\n";
            appendToDisplayHistory("\033[1;32mStarting scheduler test...");
            MainConsole::isSchedulerTestRunning = true;
        }
        else {
            std::cout << "\033[1;31mScheduler test is already running!\n";
            appendToDisplayHistory("\033[1;31mScheduler test is already running!");
        }

    }
    else if (command == "scheduler-stop") {
        if (MainConsole::isSchedulerTestRunning) {
            std::cout << "\033[1;32mStopping scheduler test...\n";
            appendToDisplayHistory("\033[1;32mStopping scheduler test...");
            MainConsole::isSchedulerTestRunning = false;

            //DEBUG: display number of processes created
            //std::cout << "Number of processes created: " << processCounter << std::endl;
        }
        else {
            std::cout << "\033[1;31mScheduler test is not running!\n";
            appendToDisplayHistory("\033[1;31mScheduler test is not running!");
        }

    }
    else if (command == "report-util") {
        std::string logFilePath = "csopesy-log.txt";

        String screenLsOutput = GlobalScheduler::getInstance()->callScreenLS();
        std::ofstream logFile(logFilePath, std::ios::out | std::ios::trunc);

        if (logFile.is_open()) {
            std::cout << "\033[1;32mReport generated at C:/" << logFilePath << "!\n";
            appendToDisplayHistory("\033[1;32mReport generated at C:/" + logFilePath + "!");
            logFile << screenLsOutput << std::endl;
            logFile.close();
        }
        else {
            std::cerr << "\033[1;31mUnable to open file: C:/" << logFilePath << "\n";
            appendToDisplayHistory("\033[1;31mUnable to open file: C:/" + logFilePath);
        }
    }
    else if (command == "clear") {
        system("cls");
		displayHistory.clear();
        this->onEnabled();
    }

    // Handle `screen -s <name>`
    else if (std::regex_search(command, match, screenCommandS)) {
        String processName = match[1].str();
        std::shared_ptr<Process> existingProcess = GlobalScheduler::getInstance()->findProcess(processName);
    
        if (existingProcess != nullptr && existingProcess->getState() != Process::FINISHED){
            std::cout << "Process with this name already exists!\n";
            appendToDisplayHistory("Process with this name already exists!");
        } else {
            std::stringstream timeStamp = createCurrentTimestamp();

            Process::RequirementFlags flags;
            flags.requireFiles = false;
            flags.numFiles = 0;
            flags.requireMemory = true;
            flags.memoryRequired = config.min_mem_per_proc +
                (std::rand() % (config.max_mem_per_proc - config.min_mem_per_proc + 1));
            std::cout<< "Memory required: " << flags.memoryRequired << std::endl;

            auto process = std::make_shared<Process>(processCounter, processName, flags);
            processCounter++;
            process->generateDummyCommands(config.min_ins, config.max_ins);
            this->addProcess(process);
            auto newScreen = std::make_shared<BaseScreen>(process, processName);

            ConsoleManager::getInstance()->registerScreen(newScreen);
            ConsoleManager::getInstance()->switchToScreen(processName);
            return;
        }
    }
    // Handle `screen -r <name>`
    else if (std::regex_search(command, match, screenCommandR)) {
        String processName = match[1].str();
        std::shared_ptr<Process> process = GlobalScheduler::getInstance()->findProcess(processName);
        if (process == nullptr)
        {
            std::cerr << "Process " << processName << " not found\n";
            appendToDisplayHistory("Process '" + processName + "' not found");
        }
        else if (process->getState() != Process::FINISHED) {
            auto newScreen = std::make_shared<BaseScreen>(process, processName);
            ConsoleManager::getInstance()->registerScreen(newScreen);
            ConsoleManager::getInstance()->switchToScreen(processName);
            return;
        }
        else {
            std::cerr << "Process " << processName << " not found\n";
            appendToDisplayHistory("Process " + processName + " not found");
        }
    }

    // Handle `screen -ls`
    else if (command == "screen -ls") {
		String screenLsOutput = GlobalScheduler::getInstance()->callScreenLS();
        std::cout << screenLsOutput;
        appendToDisplayHistory(screenLsOutput);

    }
    else {
        std::cout << "\033[1;31m" << "Error: command not recognized. Please try again\n";
		appendToDisplayHistory("\033[1;31mError: command not recognized. Please try again");
    }

	this->commandEntered = false;
    this->command.clear();

    std::cout << "\033[0m" << "Enter a command: ";
    this->inputWorker->update(true);
}

void MainConsole::enterCommand(String command)
{
    this->command = command;
    this->commandEntered = true;
}

void MainConsole::addProcess(std::shared_ptr<Process> newProcess) {
    GlobalScheduler::getInstance()->addProcess(newProcess);
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