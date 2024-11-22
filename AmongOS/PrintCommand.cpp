#include "PrintCommand.h"
#include "IThread.h"
#include <fstream>
#include <sstream>
#include <filesystem> 

PrintCommand::PrintCommand(int pid, const String& toPrint) : ICommand(pid, PRINT)
{
	this->toPrint = toPrint;
}


void PrintCommand::execute()
{
    double result = 0;
    for (int i = 1; i < 50000; ++i) {
        result += std::sin(i) * std::cos(i) / (i + 1);
    }

    volatile double preventOptimization = result;
    (void)preventOptimization;
    //std::this_thread::sleep_for(std::chrono::microseconds(10));
}