#include "IThread.h"

IThread::IThread()
{
}

void IThread::start()
{
	std::thread(&IThread::run, this).detach();
}

void IThread::sleep(int ms)
{
	std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

