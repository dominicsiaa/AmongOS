#include "PrintCommand.h"
#include "IThread.h"
#include <fstream>
#include <sstream>
#include <filesystem> 

//#include "MessageBuffer.h"


PrintCommand::PrintCommand(int pid, const String& toPrint) : ICommand(pid, PRINT)
{
	this->toPrint = toPrint;
}


void PrintCommand::execute()
{
    double result = 0;
    for (int i = 1; i < 100000; ++i) {
        result += std::sin(i) * std::cos(i) / (i + 1);
    }

    volatile double preventOptimization = result;
    (void)preventOptimization;
}