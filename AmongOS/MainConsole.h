#pragma once
#include "AConsole.h"
#include "Process.h"
#include "typedefRepo.h"
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

private:
    std::vector<std::shared_ptr<Process>> processTable;
    std::stringstream createCurrentTimestamp() const;
};