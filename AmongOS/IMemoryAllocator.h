#pragma once
#include "typedefRepo.h"

class IMemoryAllocator
{
public:
	IMemoryAllocator() = default;
	virtual ~IMemoryAllocator() = default;

	virtual void* allocate(size_t size) = 0;
	virtual void deallocate(void* ptr) = 0;
	virtual String visualizeMemory() = 0;
	virtual String visualizeProcessesInMemory() = 0;
};