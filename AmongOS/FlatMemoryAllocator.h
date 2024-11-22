#pragma once
#include <unordered_map>
#include <vector>
#include "IMemoryAllocator.h"
#include <array>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <algorithm>

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
};