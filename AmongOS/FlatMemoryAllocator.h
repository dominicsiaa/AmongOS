#pragma once
#include "IMemoryAllocator.h"
#include <chrono>
#include <list>

struct MemoryBlock {
	size_t startAddress;
	size_t endAddress;
	int processId;
	String processName;
	int commandCounter;
	std::chrono::steady_clock::time_point allocationTime;

	MemoryBlock(size_t start, size_t end, int pid, String processName, int commandCounter)
		: startAddress(start), endAddress(end), processId(pid), processName(processName), commandCounter(commandCounter),
		allocationTime(std::chrono::steady_clock::now()) {}

	size_t getSize() const {
		return endAddress - startAddress + 1;
	}
};

class FlatMemoryAllocator : public IMemoryAllocator
{
public:
	FlatMemoryAllocator(size_t maximumSize);
	~FlatMemoryAllocator() override;

	bool allocate(std::shared_ptr<Process> p) override;
	void deallocate(std::shared_ptr<Process> p) override;
	String visualizeProcessesInMemory() override;

	void mergeFree();

	size_t getUsedMemorySize() const override;
	size_t getMaximumSize() const override;
	int getNumPagedIn() const override;
	int getNumPagedOut() const override;

private:
	size_t maximumSize;
	std::list<MemoryBlock> usedMemory; 
	std::list<MemoryBlock> freeMemory;

	void initializeMemory();
	bool removeOldestBlock();
	void writeToBackingStore(String content);
	void removeFromBackingStore(int pid);
};