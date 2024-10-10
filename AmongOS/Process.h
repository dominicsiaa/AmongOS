#pragma once
#include "typedefRepo.h"

#include <sstream>

class Process
{
public:
    struct RequirementFlags
    {
        bool requireFiles;
        int numFiles;
        bool requireMmeory;
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
    void addCommand(ICommand::CommanType commandType);
    void executeCurrentCommand(); const;
    void moveToNextLine();

    bool isFinished() const;
    int getRemainingTime() const;
    int getCommandCounter() const;
    int getPID() const;
    int getCPUCoreId () const;
    ProcessState getState() const;
    String getName() cont;

    void test_generateRandomCommands(int limit);

    //~Process() = default;

    //String getName() const;
    //int getCurrentLine() const;
    //int getTotalLines() const;
    //const std::stringstream& getTimeStamp() const;
   
private:
    //String name;         
    //int currentLine;         
    //int totalLines;
    //std::stringstream timeStamp;

    int pid;
    String name;
    typedef std::vector<std::shared_ptr<ICommand>> CommandList;
    CommandList commandList;

    int commandCounter;
    int cpuCoreID = -1;
    RequirementFlags requirementFlags;
    ProcessState currentState;

    friend class ResourceEmulator;
};