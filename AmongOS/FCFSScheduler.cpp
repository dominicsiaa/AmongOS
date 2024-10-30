#include "FCFSScheduler.h"

FCFSScheduler::FCFSScheduler(int numCores, int quantumTime, std::string scheduler, unsigned int delay) : AScheduler(FCFS, -1, "FCFSScheduler") {

    this->numCores = numCores;
    for (int i = 0; i < numCores; i++)
    {
        std::shared_ptr<CPUCore> core = std::make_shared<CPUCore>(i);
        core->setDelayPerExec(delay);
        this->addCore(core);

        std::shared_ptr<CPUCoreWorker> worker = std::make_shared<CPUCoreWorker>(core);
        this->workers.push_back(worker);
        worker->update(true);
    }
    this->quantumTime = quantumTime;
    this->scheduler = scheduler;

	//create 10 processes on startup
    //for (int i = 0; i < 10; i++) {
    //    Process::RequirementFlags flags;
    //    flags.requireFiles = false;
    //    flags.numFiles = 0;
    //    flags.memoryRequired = 1000;
    //    flags.requireMemory = true;

    //    auto process = std::make_shared<Process>(i, "Process" + std::to_string(i), flags);
    //    process->generateDummyCommands(1000, 2000);
    //    this->addProcess(process);
    //}

}

FCFSScheduler* FCFSScheduler::sharedInstance = nullptr;
FCFSScheduler* FCFSScheduler::getInstance()
{
	return sharedInstance;
}

void FCFSScheduler::initialize(int numCores, int quantumTime, std::string scheduler, unsigned int delay)
{
	sharedInstance = new FCFSScheduler(numCores, quantumTime, scheduler, delay);
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
    if (scheduler == "\"fcfs\"") {
        this->doFCFS();
    }
    else if (scheduler == "\"rr\"") {
        this->doRR();
    }
}

void FCFSScheduler::doFCFS() {
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
    }
    else {
        //std::cout << "No tasks in the ready queue." << std::endl;
    }
}

void FCFSScheduler::doRR() {
    if (!readyQueue.empty()) {
        std::shared_ptr<Process> process = readyQueue.front();
        
        for (int i = 0; i < core.size(); i++) {
            if (core[i]->getTimeElapsed() >= quantumTime) {
                core[i]->clearCurrentProcess();
            }

            if (!core[i]->hasTasks()) {
                process->setCPUCoreId(i);
                process->setState(Process::RUNNING);
                ongoingProcesses.push_back(process);
                core[i]->addTask(process);
                readyQueue.pop_front();

                if (!readyQueue.empty()) {
                    process = readyQueue.front();
                }
                else {
                    break;
                }
            }
        }
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

void FCFSScheduler::addBackToRQ(std::shared_ptr<Process> process) {
    ongoingProcesses.remove(process);
    process->setState(Process::READY);
    readyQueue.push_back(process);
    //std::cout << "Task '" << process->getName() << "' added to the ready queue." << std::endl;
}

std::string FCFSScheduler::callScreenLS() {
    std::ostringstream displayStream;

    int totalCores = this->core.size();
    int usedCores = 0;

 
    for (auto core : this->core) {
        if (core->hasTasks()) {
            usedCores++;
        }
    }

    int availableCores = totalCores - usedCores;
    float cpuUtilization = (static_cast<float>(usedCores) / totalCores) * 100;

    displayStream << "CPU utilization: " << std::fixed << std::setprecision(0) << cpuUtilization << "%\n";
    displayStream << "Cores used: " << usedCores << "\n";
    displayStream << "Cores available: " << availableCores << "\n\n";

    //displayStream << "-------------------------------------\n";
    //displayStream << "Ready queue processes: \n";

    //for (auto process : readyQueue) {
    //    auto in_time_t = std::chrono::system_clock::to_time_t(process->getStartTime());
    //    std::tm buf;
    //    localtime_s(&buf, &in_time_t);

    //    std::ostringstream oss;
    //    oss << std::put_time(&buf, "%m/%d/%Y %I:%M:%S%p");

    //    displayStream << process->getName() << "    ";
    //    displayStream << "(" << oss.str() << ")    ";
    //    displayStream << "Core: " << process->getCPUCoreId() << "    ";
    //    displayStream << process->getCommandCounter() << " / " << process->getTotalCommands() << std::endl;
    //}

    displayStream << "-------------------------------------\n";
    displayStream << "Running processes: \n";

    for (auto process : ongoingProcesses) {
        auto in_time_t = std::chrono::system_clock::to_time_t(process->getStartTime());
        std::tm buf;
        localtime_s(&buf, &in_time_t);

        std::ostringstream oss;
        oss << std::put_time(&buf, "%m/%d/%Y %I:%M:%S%p");

        displayStream << process->getName() << "    ";
        displayStream << "(" << oss.str() << ")    ";
        displayStream << "Core: " << process->getCPUCoreId() << "    ";
        displayStream << process->getCommandCounter() << " / " << process->getTotalCommands() << std::endl;
    }

    displayStream << "\nFinished processes: \n";


    for (auto process : finishedProcesses) {
        auto in_time_t = std::chrono::system_clock::to_time_t(process->getStartTime());
        std::tm buf;
        localtime_s(&buf, &in_time_t);

        std::ostringstream oss;
        oss << std::put_time(&buf, "%m/%d/%Y %I:%M:%S%p");

        displayStream << process->getName() << "    ";
        displayStream << "(" << oss.str() << ")    ";
        displayStream << "Finished    ";
        displayStream << process->getCommandCounter() << " / " << process->getTotalCommands() << std::endl;
    }

    displayStream << "-------------------------------------\n";

    // Return the constructed string
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