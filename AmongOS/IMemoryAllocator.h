#pragma once
#include "typedefRepo.h"
#include "Process.h"

class IMemoryAllocator
{
public:
	IMemoryAllocator() = default;
	virtual ~IMemoryAllocator() = default;

	virtual bool allocate(std::shared_ptr<Process> p) = 0;
	virtual void deallocate(std::shared_ptr<Process> p) = 0;
	virtual String visualizeProcessesInMemory() = 0;

	virtual size_t getUsedMemorySize() const = 0;
	virtual size_t getMaximumSize() const = 0;
};