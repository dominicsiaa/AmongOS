#pragma once
#include "Process.h"
#include <iostream>
#include <memory>
#include <string>

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

private:
    static CPUCore* instance;
    int coreID;
    std::string scheduler;
    std::shared_ptr<Process> currProcess;
};