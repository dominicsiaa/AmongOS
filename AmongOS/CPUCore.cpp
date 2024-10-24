#include "CPUCore.h"
#include "FCFSScheduler.h"
#include "RRScheduler.h"

CPUCore* CPUCore::instance = nullptr;

CPUCore::CPUCore(int id) : coreID(id), scheduler(scheduler) {
    //std::cout << "CPU Core " << coreID << " initialized." << std::endl;
}

CPUCore* CPUCore::getInstance(int id) {
    if (instance == nullptr) {
        instance = new CPUCore(id);
    }
    return instance;
}

void CPUCore::addTask(const std::shared_ptr<Process>& process) {
    currProcess = process;
    process->setStartTime();
    //astd::cout << "Task '" << currProcess->getName() << "' added to CPU Core " << coreID << "." << std::endl;
}

void CPUCore::processTask() {
	if (!currProcess) {
		//std::cout << "No task to process." << std::endl;
		return;
	}
    if (currProcess->isFinished()) {
        //std::cout << "Finished task: " << currProcess->getName() << std::endl;

        clearCurrentProcess();
        return;
    }
    currProcess->executeCurrentCommand();
    currProcess->moveToNextLine();
}

int CPUCore::getCoreID() const {
    return coreID;
}

bool CPUCore::hasTasks() const {
    return currProcess != nullptr;
}

void CPUCore::clearCurrentProcess() {
    if (currProcess) {
        //std::cout << "Clearing current task: " << currProcess->getName() << std::endl;
        if (scheduler == "\"fcfs\"") {
            FCFSScheduler::getInstance()->addFinished(currProcess);
        }
        else if (scheduler == "\"rr\"") {
            RRScheduler::getInstance()->addFinished(currProcess);
        }
        currProcess.reset();
    }
}

void CPUCore::tick() {
    processTask();
}
