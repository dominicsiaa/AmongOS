#include "AScheduler.h"

AScheduler::AScheduler(SchedulingAlgorithm schedulingAlgo, int pid, String processName)
{
	this->schedulingAlgo = schedulingAlgo;
	this->pid = pid;
	this->processName = processName;
}

void AScheduler::addProcesss(std::shared_ptr<Process> process)
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
