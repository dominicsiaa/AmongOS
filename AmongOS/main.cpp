#include "ConsoleManager.h"

int main()
{
	std::srand(std::time(nullptr));

	ConsoleManager::initialize();

	bool running = true;
	while(running)
	{
		ConsoleManager::getInstance()->process();
		ConsoleManager::getInstance()->drawConsole();

		running = ConsoleManager::getInstance()->isRunning();
	}

	ConsoleManager::destroy();
	return 0;
}
