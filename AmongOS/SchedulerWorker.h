#pragma once
#include "IThread.h"
#include <string>

class SchedulerWorker : public IThread
{
public:
	SchedulerWorker() = default;
	~SchedulerWorker() = default;
	void update(bool isRunning, std::string schedulerType);
	void run() override;

private:
	bool isRunning = true;
	std::string schedulerType;
}; 
