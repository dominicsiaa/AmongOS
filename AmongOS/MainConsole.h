#pragma once
#include "AConsole.h"
#include "Process.h"
#include "typedefRepo.h"
#include "InputWorker.h"
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
    void enterCommand(String command);

    // Process Table related functions
    void addProcess(std::shared_ptr<Process> newProcess);
    void listProcesses() const;
    void appendToDisplayHistory(const std::string text);

    static bool isSchedulerTestRunning;


protected:
    void runSchedulerTest();


private:
    SchedulerWorker schedulerWorker;
    std::shared_ptr<InputWorker> inputWorker;

    bool commandEntered = false;
    String command;

    std::stringstream createCurrentTimestamp() const;
    String displayHistory;
    bool isInitialized;
    unsigned int processCounter = 0;

    struct Config {
        unsigned int num_cpu = 4;
        String scheduler = "rr"; 
        unsigned int quantum_cycles = 10;
        unsigned int batch_process_freq = 1;
        unsigned int min_ins = 1000;
        unsigned int max_ins = 2000;
        unsigned int delay_per_exec = 0;
    } config;
};