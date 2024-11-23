#pragma once
#include "ICommand.h"
#include <vector>
#include <memory>
#include <string> 
#include <sstream>
#include <chrono>

using String = std::string;

class Process
{
public:
    struct RequirementFlags
    {
        bool requireFiles;
        int numFiles;
        bool requireMemory;
        size_t memoryRequired;
    };

    enum ProcessState
    {
        READY,
        RUNNING,
        WAITING,
        FINISHED
    };

    Process(int pid, String name, RequirementFlags requirementFlags);
    ~Process() = default;

    void addCommand(ICommand::CommandType commandType);
    void generateDummyCommands(unsigned int min, unsigned int max);
    void executeCurrentCommand() const;
    void moveToNextLine();

    bool isFinished();

    int getRemainingTime() const;
    int getCommandCounter() const;
    int getPID() const;
    int getCPUCoreId() const;
    int getTotalCommands() const;
    size_t getSize() const;
    std::chrono::time_point<std::chrono::system_clock> getStartTime() const;
    ProcessState getState() const;
    String getName() const;

    void setStartTime();
    void setCPUCoreId(int coreId);
    void setState(ProcessState state);

private:
    int pid;
    String name;
    typedef std::vector<std::shared_ptr<ICommand>> CommandList;
    CommandList commandList;

    int commandCounter = 0;
    int cpuCoreID = -1;
    RequirementFlags requirementFlags;
    ProcessState currentState;
    std::chrono::time_point<std::chrono::system_clock> startTime;
};