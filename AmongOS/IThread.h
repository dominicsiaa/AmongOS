#pragma once
#include <iostream>
#include <thread>

class IThread
{
public:
	IThread();
	~IThread();

	void start();

protected:
	virtual void run() = 0;
};