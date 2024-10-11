#pragma once

#ifndef FCFSSCHEDULER_H
#define FCFSSCHEDULER_H

#include "AScheduler.h"
#include "CPUCore.h"
#include "Process.h"
#include <queue>
#include <string>
#include <iostream>

class FCFSScheduler : public AScheduler {
private:
	FCFSScheduler();
	~FCFSScheduler() = default;
	static FCFSScheduler* sharedInstance;
	std::queue<std::shared_ptr<Process>> readyQueue;
	std::list<std::shared_ptr<Process>> ongoingProcesses;
	std::list<std::shared_ptr<Process>> finishedProcesses;
	std::vector<std::shared_ptr<CPUCore>> core;
	int currentCore = 0;

public:
	static FCFSScheduler* getInstance();
	static void initialize();
	static void destroy(); 
	void addProcess(std::shared_ptr<Process> process) override;
	void addCore(std::shared_ptr<CPUCore> core);
	void addFinished(std::shared_ptr<Process> process);
	void tick() override;
	void run() override;

	void execute() override {
		std::cout << "FCFSScheduler executing." << std::endl;
	}
	void init() override {
		std::cout << "FCFSScheduler initialized." << std::endl;
	}
};

#endif // FCFSSCHEDULER_H
