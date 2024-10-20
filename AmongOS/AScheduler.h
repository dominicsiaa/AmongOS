#pragma	once

#include <unordered_map>
#include <queue>

#include "IThread.h"
#include "Process.h"
#include "TypedefRepo.h"


static const String DEBUG_SCHEDULER_NAME = "DebugScheduler";
static const String FCFS_SCHEDULER_NAME = "FCFSScheduler";
static const String SJF_NOPREEMPT_SCHEDULER_NAME = "SJF-NoPreempt-Scheduler";
static const String SJF_PREEMPT_SCHEDULER_NAME = "SJF-Preempt-Scheduler";

class AScheduler : public IThread
{
public:
	enum SchedulingAlgorithm
	{
		DEBUG,
		FCFS,
		SHORTEST_JOB_FIRST_NONPREEMPTIVE,
		SHORTEST_JOB_FIRST_PREEMPTIVE,
		ROUND_ROBIN
	};

	AScheduler(SchedulingAlgorithm schedulingAlgo, int pid, String processName);

	AScheduler() = default;

	virtual void addProcess(std::shared_ptr<Process> process) = 0;
	std::shared_ptr<Process> findProcess(String processName);
	void run() override;

	struct ProcessInfo
	{
		int pid;
		String name;
		int cpuID;
		int lineCounter;
		int linesOfCode;
		int remainingTime;
	};

	virtual void tick();

protected:
	std::queue<std::shared_ptr<Process>> readyQueue;
};