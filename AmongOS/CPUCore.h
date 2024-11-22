#pragma once
#include <iostream>
#include <memory>
#include "Process.h"
#include <mutex>

class CPUCore {
public:
    CPUCore(int id);
    static CPUCore* getInstance(int id);

    void addTask(const std::shared_ptr<Process>& process);
    void processTask();
    int getCoreID() const;
    bool hasTasks() const;
    void clearCurrentProcess();
    void tick();
    int getTimeElapsed();
    std::shared_ptr<Process> getCurrProcess();
    void setDelayPerExec(unsigned int delay) { delayPerExec = delay; tickCounter = delay; }

    std::mutex processMutex;

private:
    static CPUCore* instance;
    int coreID;
    int timeElapsed = 0;
    int tickCounter = 0;
    std::shared_ptr<Process> currProcess;
    unsigned int delayPerExec = 0;
};
