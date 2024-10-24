#include "SchedulerWorker.h"
#include "FCFSScheduler.h"

void SchedulerWorker::update(bool isRunning)
{
	this->isRunning = isRunning;
}


void SchedulerWorker::run()
{
	while (this->isRunning)
	{
		FCFSScheduler::getInstance()->tick();
		FCFSScheduler::getInstance()->sleep(200);
	}
}