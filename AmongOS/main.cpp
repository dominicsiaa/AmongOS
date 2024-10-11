#include "ConsoleManager.h"
#include "SchedulerWorker.h"
#include "FCFSScheduler.h"
#include "CPUCoreWorker.h"

int main()
{
	int coreCount = 4;
	ConsoleManager::initialize();
	FCFSScheduler::initialize();

	std::vector<std::shared_ptr<CPUCoreWorker>> workers;

	for (int i = 0; i < coreCount; i++)
	{
		std::shared_ptr<CPUCore> core = std::make_shared<CPUCore>(i);
		FCFSScheduler::getInstance()->addCore(core);

		std::shared_ptr<CPUCoreWorker> worker = std::make_shared<CPUCoreWorker>(core);
		workers.push_back(worker);
		worker->update(true);
	}

	SchedulerWorker worker;
	worker.IThread::start();
	worker.update(true);

	bool running = true;
	while(running)
	{
		ConsoleManager::getInstance()->process();
		ConsoleManager::getInstance()->drawConsole();

		running = ConsoleManager::getInstance()->isRunning();
	}

	worker.update(false);
	FCFSScheduler::destroy();
	ConsoleManager::destroy();
	return 0;
}
