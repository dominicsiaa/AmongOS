#include "IThread.h"

IThread::IThread()
{
}

IThread::~IThread()
{
}

void IThread::start()
{
	std::thread(&IThread::run, this).detach();
}

void IThread::join()
{
	std::this_thread::sleep_for(std::chrono::milliseconds(1000))
}

