#include "CPUCoreWorker.h"
#include "CPUCore.h"


CPUCoreWorker::CPUCoreWorker() : isRunning(false) {}

void CPUCoreWorker::update(bool isRunning) {
    this->isRunning = isRunning;
}

void CPUCoreWorker::run() {
    CPUCore* core = CPUCore::getInstance(1);
    if (!core) {
        std::cerr << "Error: CPU Core instance not available." << std::endl;
        return;
    }

    while (this->isRunning) {
        core->tick();
    }
}
