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
	return String();
}

size_t PagingAllocator::getUsedMemorySize() const
{
	return 0;
}

size_t PagingAllocator::getMaximumSize() const
{
	return 0;
}

