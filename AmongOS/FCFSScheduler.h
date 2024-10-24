#pragma once

#ifndef FCFSSCHEDULER_H
#define FCFSSCHEDULER_H

#include "AScheduler.h"
#include "CPUCore.h"
#include "CPUCoreWorker.h"
#include "Process.h"
#include <queue>
#include <string>
#include <iostream>

class FCFSScheduler : public AScheduler {
private:
	static FCFSScheduler* sharedInstance;

	std::list<std::shared_ptr<Process>> readyQueue;
	std::list<std::shared_ptr<Process>> ongoingProcesses;
	std::list<std::shared_ptr<Process>> finishedProcesses;

	std::vector<std::shared_ptr<CPUCore>> core;
	std::vector<std::shared_ptr<CPUCoreWorker>> workers;
	int numCores = 0;
	int currentCore = 0;

public:
	FCFSScheduler(int numCores, int timeQuantum);
	~FCFSScheduler() = default;

	static FCFSScheduler* getInstance();
	static void initialize(int numCores, int timeQuantum);
	static void destroy();

	void addProcess(std::shared_ptr<Process> process) override;
	void addCore(std::shared_ptr<CPUCore> core);
	void addFinished(std::shared_ptr<Process> process);

	String callScreenLS() override;

	std::shared_ptr<Process> findProcess(std::string name) override;

	void tick() override;
	void run() override;
};

#endif // FCFSSCHEDULER_H
