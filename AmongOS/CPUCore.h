#pragma once
#include <iostream>
#include <memory>
#include "Process.h"

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
    std::shared_ptr<Process> getCurrProcess();

private:
    static CPUCore* instance;
    int coreID;
    std::shared_ptr<Process> currProcess;
};
