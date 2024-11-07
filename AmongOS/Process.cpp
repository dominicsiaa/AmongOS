#include "Process.h"
#include <fstream>
#include <filesystem>
#include <chrono>
#include <iomanip>
#include <random>


Process::Process(int pid, String name, RequirementFlags requirementFlags)
    : pid(pid), name(name), requirementFlags(requirementFlags), commandCounter(0), currentState(READY)
{
}

void Process::addCommand(ICommand::CommandType commandType)
{
    std::shared_ptr<ICommand> command = ICommand::createCommand(commandType, pid);

    if (command) {
        commandList.push_back(command); 
    }
    else {
        
    }
}

void Process::executeCurrentCommand() const
{
    if (!this)
    {
        return;
    }

    if (currentState != FINISHED) {
        this->commandList[this->commandCounter]->execute();
    }
}

void Process::moveToNextLine()
{
    if (!this)
    {
        return;
    }

    this->commandCounter++;
}

bool Process::isFinished()
{
    if(!this)
    {
        return true;
    }

    if(this->commandCounter >= this->commandList.size())
    {
        this->currentState = FINISHED;
        return true;
    }
    return false;
}


int Process::getRemainingTime() const
{
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

int Process::getTotalCommands() const
{
	return commandList.size();
}

Process::ProcessState Process::getState() const
{
    return currentState;
}

std::string Process::getName() const
{
    return name;
}

size_t Process::getSize() const
{
    return requirementFlags.memoryRequired;
}

void Process::setCPUCoreId(int coreId)
{
	cpuCoreID = coreId;
}

void Process::setState(ProcessState state)
{
	currentState = state;
}

void Process::generateDummyCommands(unsigned int min, unsigned int max)
{
    std::random_device rd; 
    std::mt19937 gen(rd()); 
    std::uniform_int_distribution<> distr(min, max); 

    int numCommands = distr(gen);

    for (int i = 0; i < numCommands; i++) {
        addCommand(ICommand::PRINT);
    }
}

void Process::setStartTime() {
	startTime = std::chrono::system_clock::now();
}

std::chrono::time_point<std::chrono::system_clock> Process::getStartTime() const{
	return startTime;
}