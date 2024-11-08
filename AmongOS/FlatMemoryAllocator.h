#pragma once
#include <unordered_map>
#include <vector>
#include "IMemoryAllocator.h"
#include <array>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <iostream>

struct MemoryBlock{
	size_t startAddress;
	size_t endAddress;
	int processId;

	MemoryBlock(size_t start, size_t end, int pid)
		: startAddress(start), endAddress(end), processId(pid) {}

	size_t getSize() {
		return endAddress - startAddress + 1;
	}
};

class FlatMemoryAllocator : public IMemoryAllocator
{
public:
	FlatMemoryAllocator(size_t maximumSize);
	~FlatMemoryAllocator();

	bool allocate(size_t size, int pid) override;
	void deallocate(int pid) override;
	String visualizeProcessesInMemory() override;

private:
	size_t maximumSize;
	size_t allocatedSize;
	size_t numFreeBlocks;
	std::vector<char> memory;
	std::unordered_map<size_t, bool> allocationMap;
	std::vector<MemoryBlock> usedMemory;
	std::vector<MemoryBlock> freeMemory;

	void initializeMemory();
};