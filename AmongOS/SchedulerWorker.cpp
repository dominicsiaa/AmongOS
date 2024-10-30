#include "SchedulerWorker.h"
#include "FCFSScheduler.h"

void SchedulerWorker::update(bool isRunning)
{
	this->isRunning = isRunning;
}


void SchedulerWorker::run()
{
	FCFSScheduler::getInstance()->tick();

	/*while (this->isRunning)
	{
		FCFSScheduler::getInstance()->tick();
		FCFSScheduler::getInstance()->sleep(200);
	}*/
}