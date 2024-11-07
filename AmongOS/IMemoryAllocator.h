#pragma once
#include "typedefRepo.h"

class IMemoryAllocator
{
public:
	IMemoryAllocator() = default;
	virtual ~IMemoryAllocator() = default;

	virtual void* allocate(size_t size, int pid) = 0;
	virtual void deallocate(int pid) = 0;
	virtual String visualizeProcessesInMemory() = 0;
};