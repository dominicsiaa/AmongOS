#include "BaseScreen.h"
#include "ConsoleManager.h"
#include "Process.h"
#include <iostream>

#include <string>
#include <ctime>
#include <sstream>
#include <iomanip>

BaseScreen::BaseScreen(std::shared_ptr<Process> process, String processName) : AConsole(processName)
{
	this->attachedProcess = process;
	this->inputWorker = std::make_shared<InputWorker>();
}

void BaseScreen::onEnabled()
{
	this->printProcessInfo();
	std::cout << "root:\\>";

	this->command.clear();
	this->commandEntered = false;

	this->inputWorker->update(true);
}

void BaseScreen::process()
{
	if(!this->commandEntered)
	{
		return;
	}

	if(command == "clear")
	{
		system("cls");
	}
	else if(command == "process-smi")
	{
		this->printProcessInfo();
	}
	else if(command == "exit")
	{
		ConsoleManager::getInstance()->returnToPreviousConsole();
		ConsoleManager::getInstance()->unregisterScreen(this->name);
		return;
	}
	else
	{
		std::cerr << "Command not found\n";
	}

	this->commandEntered = false;
	this->command.clear();

	std::cout << "root:\\>";
	this->inputWorker->update(true);
}

void BaseScreen::enterCommand(String command)
{
	this->command = command;
	this->commandEntered = true;
}

void BaseScreen::display()
{
	
}

void BaseScreen::printProcessInfo() const
{
	// Gets the current time
	auto now = std::time(nullptr);
	std::tm localTime;
	localtime_s(&localTime, &now);

	// Format the time as MM/DD/YYYY, HH:MM:SS AM/PM
	std::stringstream timeStream;
	timeStream << std::put_time(&localTime, "%m/%d/%Y, %I:%M:%S %p");

	std::cout << "Process Name: " << this->attachedProcess->getName() << std::endl;
	//std::cout << "Process Current Line of Instruction: " << this->attachedProcess->getCurrentLine() << std::endl;
	//std::cout << "Process Total Lines of Instruction: " << this->attachedProcess->getTotalLines() << std::endl;
	std::cout << "Timestamp: " << timeStream.str() << std::endl;
}