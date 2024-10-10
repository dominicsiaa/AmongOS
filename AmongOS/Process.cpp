#include "Process.h"



Process::Process(int pid, String name, RequirementFlags requirementFlags)
    : pid(pid), name(name), requirementFlags(requirementFlags), commandCounter(0), currentState(READY)
{
}

void Process::addCommand(ICommand::CommandType commandType)
{
    // Implementation needed for adding a command
}

void Process::executeCurrentCommand() const
{
    this->commandList[this->commandCounter]->execute();
}

void Process::moveToNextLine()
{
    this->commandCounter++;
}

bool Process::isFinished() const
{
    return this->commandCounter == this->commandList.size();
}


int Process::getRemainingTime() const
{
    // Return the remaining time for the process
    return 0; 
}

int Process::getCommandCounter() const
{
    return commandCounter;
}

int Process::getPID() const
{
    return pid;
}

int Process::getCPUCoreId() const
{
    return cpuCoreID;
}

Process::ProcessState Process::getState() const
{
    return currentState;
}

std::string Process::getName() const
{
    return name;
}

void Process::test_generateRandomCommands(int limit)
{
    // Implementation for generating random commands
}