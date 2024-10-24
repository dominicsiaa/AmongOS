#include "InputWorker.h"
#include "ConsoleManager.h"

InputWorker::InputWorker(): isRunning(false) {}

void InputWorker::update(bool isRunning) {
    this->isRunning = isRunning;
    if (isRunning) {
        IThread::start();
	}
}

void InputWorker::run() {
	std::string input;
	std::getline(std::cin, input);
	ConsoleManager::getInstance()->enterCommand(input);
	this->isRunning = false;
}