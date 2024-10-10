#include "FCFSScheduler.h"

FCFSScheduler* FCFSScheduler::instance = nullptr;

FCFSScheduler* FCFSScheduler::getInstance() {
    if (instance == nullptr) {
        instance = new FCFSScheduler();
    }
    return instance;
}

void FCFSScheduler::addProcess(std::shared_ptr<Process> process) {
    readyQueue.push(process);
    std::cout << "Task '" << process->getName() << "' added to the ready queue." << std::endl;
}

void FCFSScheduler::tick() {
    if (!readyQueue.empty()) {
        std::shared_ptr<Process> process = readyQueue.front();
        readyQueue.pop();
        for (int i = 0; i < 4; i++) {
            if (!core[i].hasTasks()) {
                core[i].addTask(process);
            }
        }
    }
    else {
        std::cout << "No tasks in the ready queue." << std::endl;
    }
}