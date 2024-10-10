#pragma once
#include "IThread.h"

class SchedulerWorker : public IThread
{
public:
	SchedulerWorker() = default;
	void update(bool isRunning);
	void run() override;

private:
	bool isRunning = true;

};