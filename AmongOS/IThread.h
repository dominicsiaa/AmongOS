#pragma once
#include <iostream>
#include <thread>

class IThread
{
public:
	IThread();

	void start();
	static void sleep(int ms);

protected:
	virtual void run() = 0;
};