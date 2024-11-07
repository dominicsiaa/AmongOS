#include "GlobalScheduler.h"

#include <filesystem>

#include "FlatMemoryAllocator.h"

#include <iostream>
#include <fstream>
#include <sstream>

GlobalScheduler::GlobalScheduler(int numCores, int quantumTime, std::string scheduler, unsigned int delay, size_t max_overall_mem) : AScheduler(FCFS, -1, "GlobalScheduler") {

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

    for (int i = 0; i < 10; i++) {
        Process::RequirementFlags flags;
        flags.requireFiles = false;
        flags.numFiles = 0;
        flags.memoryRequired = 4096;
        flags.requireMemory = true;

        auto process = std::make_shared<Process>(i, "Process" + std::to_string(i), flags);
        process->generateDummyCommands(100, 100);
        this->addProcess(process);
    }


    this->memoryAllocator = std::make_shared<FlatMemoryAllocator>(max_overall_mem);
}

GlobalScheduler* GlobalScheduler::sharedInstance = nullptr;
GlobalScheduler* GlobalScheduler::getInstance()
{
	return sharedInstance;
}

void GlobalScheduler::initialize(int numCores, int quantumTime, std::string scheduler, unsigned int delay, size_t max_overall_mem)
{
	sharedInstance = new GlobalScheduler(numCores, quantumTime, scheduler, delay, max_overall_mem);
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

        if (core[i]->hasTasks() || readyQueue.empty())
        {
            continue;
        }

        std::shared_ptr<Process> process = readyQueue.front();
        process->setCPUCoreId(i);
        process->setState(Process::RUNNING);
        ongoingProcesses.push_back(process);
        core[i]->addTask(process);
        readyQueue.pop_front();
    }
}

void GlobalScheduler::doRR() {
    static int counter = 0;

    for (int i = 0; i < core.size(); i++) {

        if (core[i]->hasTasks())
        {
            if (core[i]->getCurrProcess()->getState() == Process::FINISHED)
            {
                std::shared_ptr<Process> process = core[i]->getCurrProcess();

                memoryAllocator->deallocate(process->getPID());
                core[i]->clearCurrentProcess();
                finishedProcesses.push_back(process);
                ongoingProcesses.remove(process);
            }
        }

        if (readyQueue.empty()) {
            continue;
        }

        if (core[i]->hasTasks())
        {
            if (core[i]->getTimeElapsed() >= quantumTime) {
                std::shared_ptr<Process> process = core[i]->getCurrProcess();

                memoryAllocator->deallocate(process->getPID());
                core[i]->clearCurrentProcess();
                readyQueue.push_back(process);
                ongoingProcesses.remove(process);
            }
        }

        if (!core[i]->hasTasks()) {
            std::shared_ptr<Process> process = readyQueue.front();

            if (!memoryAllocator->allocate(process->getSize(), process->getPID())) {
                readyQueue.pop_front();
                readyQueue.push_back(process);
                break;
            }

            process->setCPUCoreId(i);
            process->setState(Process::RUNNING);
            ongoingProcesses.push_back(process);
            core[i]->addTask(process);
            readyQueue.pop_front();
        }
    }

    counter++;
    if (counter == quantumTime) {
        counter = 0;
        this->quantumCounter++;

        String folder = "memory_files";
        std::ostringstream filenameStream;
        filenameStream << folder << "/memory_stamp_" << quantumCounter << ".txt";
        std::string filename = filenameStream.str();

    	std::filesystem::create_directories(folder);

        std::ofstream file(filename);

        if (file.is_open()) {
        	file << memoryAllocator->visualizeProcessesInMemory();
            file.close();
        }
        else {
            std::cerr << "Error opening file: " << filename << std::endl;
        }
    }
}

void GlobalScheduler::destroy()
{
	delete sharedInstance;
}

void GlobalScheduler::addCore(std::shared_ptr<CPUCore> core) {
    this->core.push_back(core);
   
}

void GlobalScheduler::addFinished(std::shared_ptr<Process> process) {
    ongoingProcesses.remove(process);
	finishedProcesses.push_back(process);
}

void GlobalScheduler::addBackToRQ(std::shared_ptr<Process> process) {
    ongoingProcesses.remove(process);
    readyQueue.push_back(process);
}

std::string GlobalScheduler::callScreenLS() {
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