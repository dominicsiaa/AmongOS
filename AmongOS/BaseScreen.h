#pragma once
#include "AConsole.h"
#include "TypedefRepo.h"
#include "Process.h"
#include "InputWorker.h"
#include <memory>

class BaseScreen : public AConsole
{
public:
	BaseScreen(std::shared_ptr<Process> process, String processName);
	void onEnabled() override;
	void process() override;
	void display() override;
	void enterCommand(String command) override;

private:
	void printProcessInfo() const;
	std::shared_ptr<Process> attachedProcess;
	bool refreshed = false;

	std::shared_ptr<InputWorker> inputWorker;
	bool commandEntered = false;
	String command;
};