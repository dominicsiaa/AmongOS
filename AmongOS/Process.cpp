#include "Process.h"


Process::Process(int pid, String name, RequirementFlags requirementFlags)
    : pid(pid), name(name), requirementFlags(requirementFlags), commandCounter(0), currentState(READY)
{
}

void Process::addCommand(ICommand::CommandType commandType)
{
    std::shared_ptr<ICommand> command = ICommand::createCommand(commandType, 0);

    if (command) {
        commandList.push_back(command); // Add the command to the command list
        std::cout << "Command of type " << commandType << " added to process " << name << "." << std::endl;
    }
    else {
        std::cerr << "Failed to create command of type " << commandType << " for process " << name << "." << std::endl;
    }
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
    for (int i = 0; i < limit; i++) {
        addCommand(ICommand::PRINT);
    }
}