#include "Process.h"

Process::Process(const String& processName, int totalLines)
{
	this->name = processName;
	this->currentLine = 0;
	this->totalLines = totalLines;
}

String Process::getName() const
{
	return this->name;
}

int Process::getCurrentLine() const
{
	return this->currentLine;
}

int Process::getTotalLines() const
{
	return this->totalLines;
}





