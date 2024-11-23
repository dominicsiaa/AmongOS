#include "CPUCore.h"
#include "GlobalScheduler.h"

CPUCore* CPUCore::instance = nullptr;

CPUCore::CPUCore(int id) : coreID(id) {}

CPUCore* CPUCore::getInstance(int id) {
    if (instance == nullptr) {
        instance = new CPUCore(id);
    }
    return instance;
}

void CPUCore::addTask(const std::shared_ptr<Process>& process) {
    currProcess = process;
    process->setStartTime();
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
    currProcess.reset();
    timeElapsed = 0;
}

int CPUCore::getTimeElapsed() {
    return timeElapsed;
}

void CPUCore::tick() {

    if (delayPerExec == 0) {
        processTask();
        return;
    }

    tickCounter++;

    if (tickCounter >= delayPerExec) {
        processTask();
        tickCounter = 0; 
    }
}
