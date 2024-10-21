#include "FCFSScheduler.h"

FCFSScheduler::FCFSScheduler(int numCores) : AScheduler(FCFS, -1, "FCFSScheduler") {

    this->numCores = numCores;
    for (int i = 0; i < numCores; i++)
    {
        std::shared_ptr<CPUCore> core = std::make_shared<CPUCore>(i);
        this->addCore(core);

        std::shared_ptr<CPUCoreWorker> worker = std::make_shared<CPUCoreWorker>(core);
        this->workers.push_back(worker);
        worker->update(true);
    }

	//create 10 processes on startup
    for (int i = 0; i < 10; i++) {
        Process::RequirementFlags flags;
        flags.requireFiles = false;
        flags.numFiles = 0;
        flags.memoryRequired = 1000;
        flags.requireMemory = true;

        auto process = std::make_shared<Process>(i, "Process" + std::to_string(i), flags);
        process->generateDummyCommands(100, 100);
        this->addProcess(process);
    }

}

FCFSScheduler* FCFSScheduler::sharedInstance = nullptr;
FCFSScheduler* FCFSScheduler::getInstance()
{
	return sharedInstance;
}

void FCFSScheduler::initialize(int numCores)
{
	sharedInstance = new FCFSScheduler(numCores);
}

void FCFSScheduler::addProcess(std::shared_ptr<Process> process) {
	readyQueue.push_back(process);
}

void FCFSScheduler::run() {
	while (!readyQueue.empty()) {
		tick();
	}
}

void FCFSScheduler::tick() {
    if (!readyQueue.empty()) {
        std::shared_ptr<Process> process = readyQueue.front();

        for (int i = 0; i < core.size(); i++) {
            if (!core[i]->hasTasks()) {
                process->setCPUCoreId(i);
                process->setState(Process::RUNNING);
				ongoingProcesses.push_back(process);
                core[i]->addTask(process);
				readyQueue.pop_front();
                break;
            }
        }
    } else {
        //std::cout << "No tasks in the ready queue." << std::endl;
    }
}
void FCFSScheduler::destroy()
{
	delete sharedInstance;
}

void FCFSScheduler::addCore(std::shared_ptr<CPUCore> core) {
    this->core.push_back(core);
    //std::cout << "Core " << core->getCoreID() << " added to FCFSScheduler." << std::endl;
}

void FCFSScheduler::addFinished(std::shared_ptr<Process> process) {
    ongoingProcesses.remove(process);
	finishedProcesses.push_back(process);
	//std::cout << "Task '" << process->getName() << "' added to the finished queue." << std::endl;
}

#include <iomanip>  // Include for setw

String FCFSScheduler::callScreenLS() {
    std::ostringstream displayStream;

    // Set column widths (adjust as needed)
    const int nameWidth = 20;
    const int timeWidth = 25;
    const int coreWidth = 6;
    const int commandWidth = 10;

    displayStream << "-------------------------------------\n";
    displayStream << "Running processes: \n";

    for (auto process : ongoingProcesses) {
        auto in_time_t = std::chrono::system_clock::to_time_t(process->getStartTime());
        std::tm buf;
        localtime_s(&buf, &in_time_t);

        std::ostringstream oss;
        oss << std::put_time(&buf, "%m/%d/%Y %I:%M:%S%p");
        std::string formattedTime = oss.str();

        // Process name column
        displayStream << std::setw(nameWidth) << std::left << process->getName() << "   ";

        // Formatted time column
        displayStream << "(";
        for (char c : formattedTime) {
            if (c == '/' || c == ':') {
                displayStream << "\033[0m" << c << "\033[33m";
            }
            else {
                displayStream << "\033[33m" << c;
            }
        }
        displayStream << "\033[0m)   ";

        // Core ID column
        displayStream << std::setw(coreWidth) << std::left << "Core: "
            << "\033[33m" << process->getCPUCoreId() << "\033[0m" << "   ";

        // Command counters column
        displayStream << std::setw(commandWidth) << std::left << "\033[33m"
            << process->getCommandCounter() << "\033[0m / "
            << "\033[33m" << process->getTotalCommands() << "\033[0m"
            << std::endl;
    }

    displayStream << "\nFinished processes: \n";

    for (auto process : finishedProcesses) {
        auto in_time_t = std::chrono::system_clock::to_time_t(process->getStartTime());
        std::tm buf;
        localtime_s(&buf, &in_time_t);

        std::ostringstream oss;
        oss << std::put_time(&buf, "%m/%d/%Y %I:%M:%S%p");
        std::string formattedTime = oss.str();

        // Process name column
        displayStream << std::setw(nameWidth) << std::left << process->getName() << "   ";

        // Formatted time column
        displayStream << "(";
        for (char c : formattedTime) {
            if (c == '/' || c == ':') {
                displayStream << "\033[0m" << c << "\033[33m";
            }
            else {
                displayStream << "\033[33m" << c;
            }
        }
        displayStream << "\033[0m)   ";

        // Finished status and command counters column
        displayStream << std::setw(coreWidth) << std::left << "Finished" << "   ";
        displayStream << std::setw(commandWidth) << std::left << "\033[33m"
            << process->getCommandCounter() << "\033[0m / "
            << "\033[33m" << process->getTotalCommands() << "\033[0m"
            << std::endl;
    }

    displayStream << "-------------------------------------" << std::endl;
    std::cout << displayStream.str();

    return displayStream.str();
}


std::shared_ptr<Process> FCFSScheduler::findProcess(String name) {
	for (auto process : readyQueue) {
		if (process->getName() == name) {
			return process;
		}
	}
	for (auto process : ongoingProcesses) {
		if (process->getName() == name) {
			return process;
		}
	}
	for (auto process : finishedProcesses) {
		if (process->getName() == name) {
			return process;
		}
	}
	return nullptr;
}