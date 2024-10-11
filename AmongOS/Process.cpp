#include "Process.h"
#include <fstream>
#include <filesystem>
#include <chrono>
#include <iomanip>

//TODO: update process state


Process::Process(int pid, String name, RequirementFlags requirementFlags)
    : pid(pid), name(name), requirementFlags(requirementFlags), commandCounter(0), currentState(READY)
{
}

void Process::addCommand(ICommand::CommandType commandType)
{
    std::shared_ptr<ICommand> command = ICommand::createCommand(commandType, pid);

    if (command) {
        commandList.push_back(command); // Add the command to the command list
        //std::cout << "Command of type " << commandType << " added to process " << name << "." << std::endl;
    }
    else {
        //std::cerr << "Failed to create command of type " << commandType << " for process " << name << "." << std::endl;
    }
}

void Process::executeCurrentCommand() const
{
    std::string folder = "temp"; // Specify the folder name
    std::ostringstream filename;
    filename << folder << "/pid" << pid << ".txt"; // Construct the filename with folder

    // Ensure the folder exists
    std::filesystem::create_directories(folder);

    std::ofstream outFile(filename.str(), std::ios_base::out | std::ios_base::app); // Open file in append mode, create if it doesn't exist
    if (outFile.is_open()) {
        // Check if the file is empty to write the process name
        std::ifstream inFile(filename.str());
        if (inFile.peek() == std::ifstream::traits_type::eof()) {
            outFile << "Process name: " << name << std::endl;
            outFile << "Logs:" << std::endl << std::endl;
        }
        inFile.close();

        // Get current time
        auto now = std::chrono::system_clock::now();
        auto in_time_t = std::chrono::system_clock::to_time_t(now);
        std::tm buf;
        localtime_s(&buf, &in_time_t);

        // Write log entry
        outFile << "(" << std::put_time(&buf, "%m/%d/%Y %H:%M:%S") << ") Core: " << cpuCoreID << " "; // Replace "CoreIDHere" with actual core ID
        outFile.close();
    }
    else {
        std::cerr << "Error: Could not open or create file " << filename.str() << std::endl;
    }

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

void Process::setCPUCoreId(int coreId)
{
	cpuCoreID = coreId;
}

void Process::test_generateRandomCommands(int limit)
{
    // Implementation for generating random commands
    for (int i = 0; i < limit; i++) {
        addCommand(ICommand::PRINT);
    }
}

void Process::setStartTime() {
	startTime = std::chrono::system_clock::now();
}

std::chrono::time_point<std::chrono::system_clock> Process::getStartTime() const{
	return startTime;
}