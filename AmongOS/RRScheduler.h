#pragma once

#ifndef RRSCHEDULER_H
#define RRSCHEDULER_H

#include "AScheduler.h"
#include "CPUCore.h"
#include "CPUCoreWorker.h"
#include "Process.h"
#include <list>
#include <vector>
#include <unordered_map>
#include <memory>

class RRScheduler : public AScheduler {
private:
    RRScheduler(int numCores, int timeQuantum);  // Private constructor
    ~RRScheduler() = default;

    static RRScheduler* sharedInstance;

    std::list<std::shared_ptr<Process>> readyQueue;
    std::list<std::shared_ptr<Process>> ongoingProcesses;
    std::list<std::shared_ptr<Process>> finishedProcesses;

    std::vector<std::shared_ptr<CPUCore>> core;
    std::vector<std::shared_ptr<CPUCoreWorker>> workers;
    int numCores = 0;
    int currentCore = 0;

    int timeQuantum;
    std::unordered_map<std::shared_ptr<Process>, int> cpuTimeMap;

public:
    static RRScheduler* getInstance();
    void initialize(int numCores, int timeQuantum) override;
    static void destroy();

    void addProcess(std::shared_ptr<Process> process) override;
    void addCore(std::shared_ptr<CPUCore> core);
    void addFinished(std::shared_ptr<Process> process);

    String callScreenLS() override;
    std::shared_ptr<Process> findProcess(std::string name) override;

    void tick() override;
    void run() override;
};

#endif // RRSCHEDULER_H
