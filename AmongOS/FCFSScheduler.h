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
#include <string>
#include <mutex>

class FCFSScheduler : public AScheduler {
private:
	FCFSScheduler(int numCores, int quantumTime, std::string scheduler, unsigned int delay);
	~FCFSScheduler() = default;
	static FCFSScheduler* sharedInstance;

	std::list<std::shared_ptr<Process>> readyQueue;
	std::list<std::shared_ptr<Process>> ongoingProcesses;
	std::list<std::shared_ptr<Process>> finishedProcesses;

	std::vector<std::shared_ptr<CPUCore>> core;
	std::vector<std::shared_ptr<CPUCoreWorker>> workers;

	std::string scheduler;
	int	quantumTime = 0;
	int numCores = 0;
	int currentCore = 0;

	//std::mutex queueMutex;

	void doFCFS();
	void doRR();

public:
	static FCFSScheduler* getInstance();
	static void initialize(int numCores, int quantumTime, std::string scheduler, unsigned int delay);
	static void destroy(); 

	void addProcess(std::shared_ptr<Process> process) override;
	void addCore(std::shared_ptr<CPUCore> core);
	void addFinished(std::shared_ptr<Process> process);
	void addBackToRQ(std::shared_ptr<Process> process);

	String callScreenLS();

	std::shared_ptr<Process> findProcess(String name);

	void tick() override;
	void run() override;
};

#endif // FCFSSCHEDULER_H
