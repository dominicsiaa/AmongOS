#include "CPUCoreWorker.h"

CPUCoreWorker::CPUCoreWorker(std::shared_ptr<CPUCore> core) : core(core), isRunning(false) {}

void CPUCoreWorker::update(bool isRunning) {
    this->isRunning = isRunning;
    if (isRunning) {
        IThread::start();
    }
}

void CPUCoreWorker::run() {
    if (!core) {
        std::cerr << "Error: CPU Core instance not available." << std::endl;
        return;
    }

    core->tick();

    //while (this->isRunning) {
    //    core->tick();
    //    // IThread::sleep(100); // Sleep to simulate work and prevent busy-waiting
    //}
}