#pragma once
#pragma once
#include "IMemoryAllocator.h"
#include <chrono>
#include <list>

struct ProcessInfo {
	std::shared_ptr<Process> process;
	std::chrono::steady_clock::time_point allocationTime;

	ProcessInfo(std::shared_ptr<Process> p)
		: process(p),
		allocationTime(std::chrono::steady_clock::now()) {}
};

class PagingAllocator : public IMemoryAllocator
{
public:
	PagingAllocator(size_t maximumSize, size_t memPerFrame);
	~PagingAllocator() override;

	bool allocate(std::shared_ptr<Process> p) override;
	void deallocate(std::shared_ptr<Process> p) override;
	String visualizeProcessesInMemory() override;

	size_t getUsedMemorySize() const override;
	size_t getMaximumSize() const override;

private:
	size_t maximumSize;
	size_t numFrames;
	size_t frameSize;

	std::list<size_t> freeFrameList;
	std::list<ProcessInfo> processList;

	bool removeOldestProcess();
	void writeToBackingStore(String content);
	void removeFromBackingStore(int pid);
};