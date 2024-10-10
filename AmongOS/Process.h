#pragma once
//#include "TypedefRepo.h"
#include "ICommand.h"
#include <vector>
#include <memory>
#include <string> 
#include <sstream>

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

    void addCommand(ICommand::CommandType commandType);
    void executeCurrentCommand() const;
    void moveToNextLine();

    bool isFinished() const;
    int getRemainingTime() const;
    int getCommandCounter() const;
    int getPID() const;
    int getCPUCoreId() const;
    ProcessState getState() const;
    String getName() const;

    void test_generateRandomCommands(int limit);

private:
    int pid;
    String name;
    typedef std::vector<std::shared_ptr<ICommand>> CommandList;
    CommandList commandList;

    int commandCounter = 0;
    int cpuCoreID = -1;
    RequirementFlags requirementFlags;
    ProcessState currentState;

    //friend class ResourceEmulator;
};