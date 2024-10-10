
#pragma once
#include "ICommand.h"
#include "TypedefRepo.h"

class PrintCommand : public ICommand
{
public:
	PrintCommand(int pid, const String& toPrint);
	void execute() override;
private:
	String toPrint;
};