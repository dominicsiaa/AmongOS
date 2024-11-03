#include "CPUCore.h"
#include "GlobalScheduler.h"

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
		return;
	}

    if (currProcess->isFinished()) {
        return;
    }

    currProcess->executeCurrentCommand();
    currProcess->moveToNextLine();
    if (currProcess->isFinished()) {
        return;
    }

    timeElapsed++;
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
    //if (currProcess) {
    //    //std::cout << "Clearing current task: " << currProcess->getName() << std::endl;
    //    if (currProcess->isFinished()) {
    //        GlobalScheduler::getInstance()->addFinished(currProcess);
    //    }
    //    else {
    //        GlobalScheduler::getInstance()->addBackToRQ(currProcess);
    //    }

    //    currProcess.reset();
    //    timeElapsed = 0;
    //}

    //toClear = true;

    currProcess.reset();
    timeElapsed = 0;
}

int CPUCore::getTimeElapsed() {
    return timeElapsed;
}

void CPUCore::tick() {
    /*if (toClear) {
    	clearCurrentProcess();
		toClear = false;
    }*/

    /*if(toClear)
    {
        currProcess.reset();
        timeElapsed = 0;
    }*/

    if (delayPerExec == 0) {
        processTask();
        return;
    }

    tickCounter++;

    if (tickCounter >= delayPerExec) {
        processTask();
        tickCounter = 0; 
    }
    // std::this_thread::sleep_for(std::chrono::milliseconds(100));
}
