#include "CPUCore.h"

CPUCore* CPUCore::instance = nullptr;

CPUCore::CPUCore(int id) : coreID(id) {
    std::cout << "CPU Core " << coreID << " initialized." << std::endl;
}

CPUCore* CPUCore::getInstance(int id) {
    if (instance == nullptr) {
        instance = new CPUCore(id);
    }
    return instance;
}

void CPUCore::addTask(const std::shared_ptr<Process>& process) {
    currProcess = process;
    std::cout << "Task '" << currProcess->getName() << "' added to CPU Core " << coreID << "." << std::endl;
}

void CPUCore::processTask() {
    if (currProcess->isFinished()) {
        std::cout << "Finished task: " << currProcess->getName() << std::endl;

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
        std::cout << "Clearing current task: " << currProcess->getName() << std::endl;
        currProcess.reset();
    }
}

void CPUCore::tick() {
    processTask();
}
