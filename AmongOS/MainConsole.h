#pragma once
#include "AConsole.h"
#include "Process.h"
#include "typedefRepo.h"
#include "SchedulerWorker.h"
#include "FCFSScheduler.h"
#include <vector>
#include <memory>

class MainConsole : public AConsole
{
public:
    MainConsole();
    ~MainConsole() = default;

    void onEnabled() override;
    void display() override;
    void process() override;

    // Process Table related functions
    void addProcess(std::shared_ptr<Process> newProcess);
    void listProcesses() const;
    void appendToDisplayHistory(const std::string text);

private:
    SchedulerWorker schedulerWorker;
    std::vector<std::shared_ptr<Process>> processTable;
    std::stringstream createCurrentTimestamp() const;
    String displayHistory;
    bool isInitialized;

    struct Config {
        unsigned int num_cpu = 4;
        std::string scheduler = "rr"; 
        unsigned int quantum_cycles = 5;
        unsigned int batch_process_freq = 1;
        unsigned int min_ins = 1000;
        unsigned int max_ins = 2000;
        unsigned int delays_per_exec = 0;
    } config;
};