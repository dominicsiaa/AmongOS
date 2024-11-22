#pragma once
#include "AConsole.h"
#include "Process.h"
#include "typedefRepo.h"
#include "InputWorker.h"
#include "SchedulerWorker.h"
#include "GlobalScheduler.h"
#include <vector>
#include <memory>

class MainConsole : public AConsole 
{
public:
    MainConsole();
    ~MainConsole() = default;

    void onEnabled() override;
    void process() override;
    void enterCommand(String command);

    void addProcess(std::shared_ptr<Process> newProcess);
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
    unsigned int processCounter = 0;

    bool isInitialized;
    struct Config {
        unsigned int num_cpu = 4;
        String scheduler = "rr"; 
        unsigned int quantum_cycles = 10;
        unsigned int batch_process_freq = 1;
        unsigned int min_ins = 1000;
        unsigned int max_ins = 2000;
        unsigned int delay_per_exec = 0;
        size_t max_overall_mem = 16384;
        size_t mem_per_frame = 16;
        size_t min_mem_per_proc = 4096;
        size_t max_mem_per_proc = 4096;

    } config;
};