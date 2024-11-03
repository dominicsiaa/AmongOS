#include "SchedulerWorker.h"
#include "GlobalScheduler.h"

void SchedulerWorker::update(bool isRunning)
{
	this->isRunning = isRunning;
}


void SchedulerWorker::run()
{
	GlobalScheduler::getInstance()->tick();

	/*while (this->isRunning)
	{
		GlobalScheduler::getInstance()->tick();
		GlobalScheduler::getInstance()->sleep(200);
	}*/
}