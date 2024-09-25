#include "BaseScreen.h"
#include "ConsoleManager.h"
#include "Process.h"
#include <iostream>

BaseScreen::BaseScreen(std::shared_ptr<Process> process, String processName) : AConsole(processName)
{
	this->attachedProcess = process;
}

void BaseScreen::onEnabled()
{

}

void BaseScreen::process()
{
	if (!this->refreshed)
	{
		this->refreshed = true;
		this->printProcessInfo();
	}

	std::cout << "root:\\>";

	String command;
	std::getline(std::cin, command);

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
	}
	else
	{
		std::cerr << "Command not found\n";
	}
}

void BaseScreen::display()
{
	
}

void BaseScreen::printProcessInfo() const
{
	//TODO: Im just guessing here
	std::cout << "Process Name: " << this->attachedProcess->getName() << std::endl;
	std::cout << "Process Current Line of Instruction: " << this->attachedProcess->getCurrentLine() << std::endl;
	std::cout << "Process Total Lines of Instruction: " << this->attachedProcess->getTotalLines() << std::endl;
}