#pragma once
#include <iostream>
#include <memory>
#include "Process.h"

class CPUCore {
public:
    static CPUCore* getInstance(int id);

    CPUCore() : coreID(-1) {}

    void addTask(const std::shared_ptr<Process>& process);
    void processTask();
    int getCoreID() const;
    bool hasTasks() const;
    void clearCurrentProcess();
    void tick();

private:
    CPUCore(int id);
    static CPUCore* instance;
    int coreID;
    std::shared_ptr<Process> currProcess;
};
