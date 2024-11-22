#pragma once
#include <vector>
#include "IMemoryAllocator.h"
#include <chrono>

struct MemoryBlock {
	size_t startAddress;
	size_t endAddress;
	int processId;
	std::chrono::steady_clock::time_point allocationTime;  // Time of allocation

	MemoryBlock(size_t start, size_t end, int pid)
		: startAddress(start), endAddress(end), processId(pid),
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

	bool allocate(size_t size, int pid) override;
	void deallocate(int pid) override;
	String visualizeProcessesInMemory() override;

	void mergeFree();

private:
	size_t maximumSize;
	std::vector<MemoryBlock> usedMemory;
	std::vector<MemoryBlock> freeMemory;

	void initializeMemory();
	void removeOldestBlock();
};