#include "PrintCommand.h"
#include "IThread.h"
//#include "MessageBuffer.h"


PrintCommand::PrintCommand(int pid, const String& toPrint) : ICommand(pid, PRINT)
{
	this->toPrint = toPrint;
}


void PrintCommand::execute()
{
	std::cout << "PID: " << pid << ", Message: " << toPrint << std::endl;

	//std::stringstream msg; msg << String("PID ") << this->pid << ":" << this->toPrint << std::endl;
	//MessageBuffer::log(msg.str());
}