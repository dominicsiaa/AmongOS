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
	std::cout << "Process Name: " << this->attachedProcess->getName() << std::endl;
	std::cout << "ID: " << this->attachedProcess->getPID() << std::endl;
	std::cout << std::endl;
	std::cout << "Current instruction line: " << this->attachedProcess->getCommandCounter() << std::endl;
	std::cout << "Lines of code: " << this->attachedProcess->getTotalCommands() << std::endl << std::endl;
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


void BaseScreen::printProcessInfo() const
{
	if (this->attachedProcess->isFinished())
	{
		std::cout << std::endl;
		std::cout << "Process Name: " << this->attachedProcess->getName() << std::endl;
		std::cout << "ID: " << this->attachedProcess->getPID() << std::endl << std::endl;
		std::cout << "Finished!" << std::endl << std::endl;
	}
	else {
		std::cout << std::endl;
		std::cout << "Process Name: " << this->attachedProcess->getName() << std::endl;
		std::cout << "ID: " << this->attachedProcess->getPID() << std::endl << std::endl;
		std::cout << "Current instruction line: " << this->attachedProcess->getCommandCounter() << std::endl;
		std::cout << "Lines of code: " << this->attachedProcess->getTotalCommands() << std::endl << std::endl;
	
	}
}