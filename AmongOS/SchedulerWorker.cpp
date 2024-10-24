#include "SchedulerWorker.h"
#include "FCFSScheduler.h"
#include "RRScheduler.h"

void SchedulerWorker::update(bool isRunning, std::string schedulerType) {
	this->isRunning = isRunning;
	this->schedulerType = schedulerType;
}


void SchedulerWorker::run()
{
	while (this->isRunning)
	{
		if (this->schedulerType == "\"fcfs\"") {
			FCFSScheduler::getInstance()->tick();
			FCFSScheduler::getInstance()->sleep(200);
		}
		else if (this->schedulerType == "\"rr\"") {
			RRScheduler::getInstance()->tick();
			RRScheduler::getInstance()->sleep(200);
		}
	}
}