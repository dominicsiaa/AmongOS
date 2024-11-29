#include "PagingAllocator.h"
#include <sstream>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <filesystem>

#include "GlobalScheduler.h"

PagingAllocator::PagingAllocator(size_t maximumSize, size_t memPerFrame)
{
	this->maximumSize = maximumSize;
	numFrames = maximumSize / memPerFrame;
	frameSize = memPerFrame;

	for (size_t i = 1; i <= numFrames; i++) {
		freeFrameList.push_back(i);
	}

	std::cout << "Paging Allocator initialized with " << numFrames << " frames of size " << frameSize << std::endl;
}

PagingAllocator::~PagingAllocator()
{
}

bool PagingAllocator::allocate(std::shared_ptr<Process> p)
{
	size_t size = p->getSize();
	int pid = p->getPID();

	// if process is too big for memory, return false
	if (size > maximumSize) {
		return false;
	}

	// for each page in process page table which is an unordered map
	for (auto& page : p->pageTable) {
		// if page is already in memory, return true
		if (page.second != 0) {
			continue;
		}

		// if there are no free frames, return false
		if (freeFrameList.empty()) {
			removeOldestProcess();
		}

		// get the first free frame
		size_t frame = freeFrameList.front();
		freeFrameList.pop_front();

		// set the page table entry to the frame number
		page.second = frame;
	}

	processList.push_back(p);
	return true;
}

bool PagingAllocator::removeOldestProcess()
{
	if (processList.empty()) {
		return false;
	}

	auto oldestProcess = processList.begin();
	for (auto it = processList.begin(); it != processList.end(); ++it) {
		if (it->allocationTime < oldestProcess->allocationTime) {
			oldestProcess = it;
		}
	}

	deallocate(oldestProcess->process);
	//TODO: loop through process list and remove the oldest process

	
	return true;
}

void PagingAllocator::deallocate(std::shared_ptr<Process> p)
{
	for (auto& page : p->pageTable) {
		if (page.second == 0) {
			continue;
		}

		freeFrameList.push_back(page.second);
		page.second = 0;
	}

	processList.remove_if([p](const ProcessInfo& pi) {
		return pi.process == p;
	});
}

String PagingAllocator::visualizeProcessesInMemory()
{
	size_t totalMemory = maximumSize;
	size_t usedMemorySize = getUsedMemorySize();
	float memoryUtilization = (static_cast<float>(usedMemorySize) / totalMemory) * 100;
	std::ostringstream visualizedMemory;

	visualizedMemory << "\n";
	visualizedMemory << "--------------------------------------------" << "\n";
	visualizedMemory << "| PROCESS-SMI V01.00 Driver Version: 01.00 |" << "\n";
	visualizedMemory << "--------------------------------------------" << "\n";
	visualizedMemory << "CPU utilization: " << GlobalScheduler::getInstance()->getCPUUtilization() << "%" << "\n";
	visualizedMemory << "Memory Usage: " << usedMemorySize << "KB / " << totalMemory << "KB\n";
	visualizedMemory << "Memory Utilization: " << std::fixed << std::setprecision(0) << memoryUtilization << "%\n";
	visualizedMemory << "\n";
	visualizedMemory << "============================================" << "\n";
	visualizedMemory << "Running Processes and memory usage: " << "\n";
	visualizedMemory << "--------------------------------------------" << "\n";
	for (const auto& processInfo : processList) {
		const auto& process = processInfo.process;
		const auto& pageTable = process->pageTable;

		// Count the number of allocated pages
		size_t allocatedPages = std::count_if(
			pageTable.begin(), pageTable.end(),
			[](const std::pair<size_t, size_t>& page) {
				return page.second != 0; // Check if the page is allocated (non-zero frame)
			});

		// Print process name and memory usage
		visualizedMemory << process->getName() << "  ";
		visualizedMemory << (allocatedPages * frameSize) << "KB\n";
	}
	visualizedMemory << "--------------------------------------------" << "\n";

	return visualizedMemory.str();

}

size_t PagingAllocator::getUsedMemorySize() const
{
	size_t usedMemory = 0;

	// Iterate through all processes in the processList
	for (const auto& processInfo : processList) {
		const auto& pageTable = processInfo.process->pageTable;

		// Count the number of allocated pages
		size_t allocatedPages = std::count_if(
			pageTable.begin(), pageTable.end(),
			[](const std::pair<size_t, size_t>& page) {
				return page.second != 0; // Check if the page is allocated (non-zero frame)
			});

		// Add the memory used by this process to the total
		usedMemory += allocatedPages * frameSize;
	}

	return usedMemory;
}

size_t PagingAllocator::getMaximumSize() const
{
	return 0;
}

