#include "AScheduler.h"

AScheduler::AScheduler(SchedulingAlgorithm schedulingAlgo, int pid, String processName)
{

}

void AScheduler::addProcess(std::shared_ptr<Process> process)
{
}

std::shared_ptr<Process> AScheduler::findProcess(String processName)
{
	return std::shared_ptr<Process>();
}

void AScheduler::run()
{
}

void AScheduler::stop()
{
}

// Process the next task in the ready queue
void AScheduler::tick() {
}