#pragma once

#ifndef FCFSSCHEDULER_H
#define FCFSSCHEDULER_H

#include "AScheduler.h"
#include "CPUCore.h"
#include "Process.h"
#include <queue>
#include <string>
#include <iostream>

class FCFSScheduler : public AScheduler {
private:
private:
    static FCFSScheduler* instance;
    CPUCore core;

    FCFSScheduler() : AScheduler(FCFS, -1, "FCFSScheduler") {}

public:
    static FCFSScheduler* getInstance();

    void addProcess(std::shared_ptr<Process> process) override;

    void tick() override;

    void init() override {
        std::cout << "FCFSScheduler initialized." << std::endl;
    }

    void execute() override {
        std::cout << "FCFSScheduler executing." << std::endl;
    }
};

#endif // FCFSSCHEDULER_H
