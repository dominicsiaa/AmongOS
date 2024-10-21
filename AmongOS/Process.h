#pragma once
//#include "TypedefRepo.h"
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
        int memoryRequired;
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
    void executeCurrentCommand() const;
    void moveToNextLine();
    void setStartTime();

    std::chrono::time_point<std::chrono::system_clock> getStartTime() const;

    bool isFinished();
    int getRemainingTime() const;
    int getCommandCounter() const;
    int getPID() const;
    int getCPUCoreId() const;
    int getTotalCommands() const;
    ProcessState getState() const;
    String getName() const;

    void setCPUCoreId(int coreId);

    void generateDummyCommands(unsigned int min, unsigned int max);

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

    //friend class ResourceEmulator;
};