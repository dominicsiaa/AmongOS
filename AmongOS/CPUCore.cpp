#include "CPUCore.h"
#include "FCFSScheduler.h"

CPUCore* CPUCore::instance = nullptr;

CPUCore::CPUCore(int id) : coreID(id) {
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

std::shared_ptr<Process> CPUCore::getCurrProcess() {
    return this->currProcess;
}

bool CPUCore::hasTasks() const {
    return currProcess != nullptr;
}

void CPUCore::clearCurrentProcess() {
    if (currProcess) {
        //std::cout << "Clearing current task: " << currProcess->getName() << std::endl;
        if (currProcess->isFinished()) {
            FCFSScheduler::getInstance()->addFinished(currProcess);
        }
        else {
            FCFSScheduler::getInstance()->addBackToRQ(currProcess);
        }

        currProcess.reset();
        timeElapsed = 0;
    }
}

int CPUCore::getTimeElapsed() {
    return timeElapsed;
}

void CPUCore::tick() {
    processTask();
    timeElapsed++;
    // std::this_thread::sleep_for(std::chrono::milliseconds(100));
}
