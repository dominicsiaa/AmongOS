#pragma once
#include "typedefRepo.h"
#include "Process.h"

class IMemoryAllocator
{
public:
	IMemoryAllocator() = default;
	virtual ~IMemoryAllocator() = default;

	virtual bool allocate(std::shared_ptr<Process> p) = 0;
	virtual void deallocate(int pid) = 0;
	virtual String visualizeProcessesInMemory() = 0;
};