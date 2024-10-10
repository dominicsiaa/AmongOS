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

