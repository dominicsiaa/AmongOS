#include "PrintCommand.h"
#include "IETThread.h"
#include "MessageBuffer.h"


PrintCommand::PrintCommand(int pid, String& toPrint) : ICommand(pid, PRINT)
{
	this->toPrint = toPrint;
}


void PrintCommand::execute()
{
	ICommand::execute();

	std::stringstream msg; msg << String("PID ") << this->pid << ":" << this->toPrint << std::endl;
	MessageBuffer::log(msg.str());
}