#include "Process.h"

Process::Process(const String& processName, int totalLines, std::stringstream timeStamp)
{
	this->name = processName;
	this->currentLine = 0;
	this->totalLines = totalLines;
	this->timeStamp = std::move(timeStamp);
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

const std::stringstream& Process::getTimeStamp() const
{
    return this->timeStamp;
}



