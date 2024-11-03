#include "GlobalScheduler.h"

GlobalScheduler::GlobalScheduler(int numCores, int quantumTime, std::string scheduler, unsigned int delay) : AScheduler(FCFS, -1, "GlobalScheduler") {

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

GlobalScheduler* GlobalScheduler::sharedInstance = nullptr;
GlobalScheduler* GlobalScheduler::getInstance()
{
	return sharedInstance;
}

void GlobalScheduler::initialize(int numCores, int quantumTime, std::string scheduler, unsigned int delay)
{
	sharedInstance = new GlobalScheduler(numCores, quantumTime, scheduler, delay);
}

void GlobalScheduler::addProcess(std::shared_ptr<Process> process) {
	readyQueue.push_back(process);
}

void GlobalScheduler::run() {
	while (!readyQueue.empty()) {
		tick();
	}
}

void GlobalScheduler::tick() {
    if (scheduler == "\"fcfs\"") {
        this->doFCFS();
    }
    else if (scheduler == "\"rr\"") {
        this->doRR();
    }
}

void GlobalScheduler::doFCFS() {
    //if (!readyQueue.empty()) {
    //    std::shared_ptr<Process> process = readyQueue.front();

    //    for (int i = 0; i < core.size(); i++) {
    //        if (!core[i]->hasTasks()) {
    //            process->setCPUCoreId(i);
    //            process->setState(Process::RUNNING);
    //            ongoingProcesses.push_back(process);
    //            core[i]->addTask(process);
    //            readyQueue.pop_front();
    //            break;
    //        }
    //    }
    //}
    //else {
    //    //std::cout << "No tasks in the ready queue." << std::endl;
    //}

    for (int i = 0; i < core.size(); i++) {

        if (core[i]->getCurrProcess() != nullptr)
        {
            if (core[i]->getCurrProcess()->getState() == Process::FINISHED)
            {
                finishedProcesses.push_back(core[i]->getCurrProcess());
                ongoingProcesses.remove(core[i]->getCurrProcess());
                core[i]->clearCurrentProcess();
            }
        }

        /*if(core[i]->toClear)
        {
            continue;
        }

        if(core[i]->hasTasks())
        {
            if (core[i]->getCurrProcess()->getState() == Process::FINISHED)
            {
                core[i]->toClear = true;
            }
        }*/

        if (core[i]->hasTasks() || readyQueue.empty())
        {
            continue;
        }

        //std::lock_guard<std::mutex> lock(queueMutex);
        std::shared_ptr<Process> process = readyQueue.front();
        process->setCPUCoreId(i);
        process->setState(Process::RUNNING);
        ongoingProcesses.push_back(process);
        core[i]->addTask(process);
        readyQueue.pop_front();
    }
}

void GlobalScheduler::doRR() {
    // DANES IMPLEMENTATION

    //if (!readyQueue.empty()) {
    //    std::shared_ptr<Process> process = readyQueue.front();
    //    
    //    for (int i = 0; i < core.size(); i++) {
    //        if (core[i]->getTimeElapsed() >= quantumTime) {
    //            core[i]->clearCurrentProcess();
    //        }

    //        if (!core[i]->hasTasks()) {
    //            process->setCPUCoreId(i);
    //            process->setState(Process::RUNNING);
    //            ongoingProcesses.push_back(process);
    //            core[i]->addTask(process);
    //            readyQueue.pop_front();

    //            if (!readyQueue.empty()) {
    //                process = readyQueue.front();
    //            }
    //            else {
    //                break;
    //            }
    //        }
    //    }
    //}

    // DOMS CORRECTION

    //for (int i = 0; i < core.size(); i++) {
    //    if (core[i]->toClear)
    //    {
    //        continue;
    //    }

    //    if (core[i]->hasTasks())
    //    {
    //        if (core[i]->getCurrProcess()->getState() == Process::FINISHED)
    //        {
    //            core[i]->toClear = true;
    //        }
    //    }

    //    if (readyQueue.empty()) {
    //        continue;
    //    }

    //    if (core[i]->getTimeElapsed() >= quantumTime) {
    //        core[i]->toClear = true;
    //    }

    //    if (!core[i]->hasTasks()) {
    //        //std::lock_guard<std::mutex> lock(queueMutex);
    //        std::shared_ptr<Process> process = readyQueue.front();

    //        process->setCPUCoreId(i);
    //        process->setState(Process::RUNNING);
    //        ongoingProcesses.push_back(process);
    //        core[i]->addTask(process);
    //        readyQueue.pop_front();
    //    }
    //}

    // NEW IMPLEMENTATION

    for (int i = 0; i < core.size(); i++) {

        if (core[i]->hasTasks())
        {
            if (core[i]->getCurrProcess()->getState() == Process::FINISHED)
            {
                std::shared_ptr<Process> process = core[i]->getCurrProcess();
                core[i]->clearCurrentProcess();
                finishedProcesses.push_back(process);
                ongoingProcesses.remove(process);

                //std::cout << "Task '" << process->getName() << "' finished with " << process->getCommandCounter() << "/ " << process->getTotalCommands() << " instructions." << std::endl;
            }
        }

        if (readyQueue.empty()) {
            continue;
        }

        if (core[i]->hasTasks())
        {
            if (core[i]->getTimeElapsed() >= quantumTime) {
                std::shared_ptr<Process> process = core[i]->getCurrProcess();
                core[i]->clearCurrentProcess();
                readyQueue.push_back(process);
                ongoingProcesses.remove(process);

                //std::cout << "Task '" << process->getName() << "' added back to the ready queue with " << process->getCommandCounter() << "/ " << process->getTotalCommands() << " instructions." << std::endl;
            }
        }

        if (!core[i]->hasTasks()) {
            //std::lock_guard<std::mutex> lock(queueMutex);
            std::shared_ptr<Process> process = readyQueue.front();

            process->setCPUCoreId(i);
            process->setState(Process::RUNNING);
            ongoingProcesses.push_back(process);
            core[i]->addTask(process);
            readyQueue.pop_front();
        }
    }
}

void GlobalScheduler::destroy()
{
	delete sharedInstance;
}

void GlobalScheduler::addCore(std::shared_ptr<CPUCore> core) {
    this->core.push_back(core);
    //std::cout << "Core " << core->getCoreID() << " added to GlobalScheduler." << std::endl;
}

void GlobalScheduler::addFinished(std::shared_ptr<Process> process) {
    //std::lock_guard<std::mutex> lock(queueMutex);
    ongoingProcesses.remove(process);
	finishedProcesses.push_back(process);
	//std::cout << "Task '" << process->getName() << "' added to the finished queue." << std::endl;
}

void GlobalScheduler::addBackToRQ(std::shared_ptr<Process> process) {
    //std::lock_guard<std::mutex> lock(queueMutex);
    ongoingProcesses.remove(process);
    //process->setState(Process::READY);
    readyQueue.push_back(process);
    //std::cout << "Task '" << process->getName() << "' added to the ready queue." << std::endl;
}

std::string GlobalScheduler::callScreenLS() {
    //std::lock_guard<std::mutex> lock(queueMutex);

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




std::shared_ptr<Process> GlobalScheduler::findProcess(String name) {
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