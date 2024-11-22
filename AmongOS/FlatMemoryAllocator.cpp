#include "FlatMemoryAllocator.h"
#include <sstream>

FlatMemoryAllocator::FlatMemoryAllocator(size_t maximumSize)
	: maximumSize(maximumSize)
{
	initializeMemory();
}

FlatMemoryAllocator::~FlatMemoryAllocator()
{
}

bool compareByAddress(const MemoryBlock& a, const MemoryBlock& b) {
	return a.startAddress < b.startAddress;
}


void FlatMemoryAllocator::mergeFree()
{
	if (freeMemory.size() <= 1) {
		return;
	}

	std::sort(freeMemory.begin(), freeMemory.end(), [](const MemoryBlock& a, const MemoryBlock& b) {
		return a.startAddress < b.startAddress;
	});

	for (size_t i = 0; i < freeMemory.size(); i++) {
		if (i > freeMemory.size() - 2) {
			break;
		}
		if (freeMemory[i].endAddress + 1 == freeMemory[i + 1].startAddress) {
			freeMemory[i].endAddress = freeMemory[i + 1].endAddress;
			freeMemory.erase(freeMemory.begin() + i + 1);
			--i;
			break;
		}
	}
}

bool FlatMemoryAllocator::allocate(size_t size, int pid)
{
	mergeFree();

	for (size_t i = 0; i < usedMemory.size(); i++) {
		if (pid == usedMemory[i].processId) {
			return true;
		}
	}

	for (size_t i = 0; i < freeMemory.size(); i++) {
		size_t blockSize = freeMemory[i].getSize();

		if (size > blockSize) {
			continue;
		}

		size_t startAddress = freeMemory[i].startAddress;
		size_t endAddress = startAddress + size - 1;
		usedMemory.push_back(MemoryBlock(startAddress, endAddress, pid));

		if (blockSize == size) {
			freeMemory.erase(freeMemory.begin() + i);
		}
		else {
			freeMemory[i].startAddress = endAddress + 1;
		}

		std::sort(usedMemory.begin(), usedMemory.end(), [](const MemoryBlock& a, const MemoryBlock& b) {
			return a.startAddress > b.startAddress;
		});

		return true;
	}
	return false;
}

void FlatMemoryAllocator::deallocate(int pid)
{
	for (size_t i = 0; i < usedMemory.size(); i++) {
		if (pid != usedMemory[i].processId) {
			continue;
		}

		MemoryBlock deallocatedBlock = usedMemory[i];
		usedMemory.erase(usedMemory.begin() + i);
		freeMemory.push_back(deallocatedBlock);
	}

	mergeFree();
}

String FlatMemoryAllocator::visualizeProcessesInMemory()
{
	std::ostringstream memCho;

	auto now = std::chrono::system_clock::now();
	std::time_t timeStamp = std::chrono::system_clock::to_time_t(now);

	std::tm timeInfo;
	localtime_s(&timeInfo, &timeStamp);

	size_t externFrag = 0;
	mergeFree();
	for (size_t i = 0; i < freeMemory.size(); i++) {
		if (freeMemory[i].endAddress == maximumSize - 1) {
			continue;
		}
		externFrag += freeMemory[i].getSize();
	}

	int numProc = usedMemory.size();

	memCho << "Timestamp: " << std::put_time(&timeInfo, "(%m/%d/%Y %I:%M:%S%p)") << "\n";
	memCho << "Number of processes in memory: " << numProc << "\n";
	memCho << "Total external fragmentation in KB: " << externFrag << "\n\n";

	memCho << "----end---- = " << maximumSize << "\n\n";

	for (size_t i = 0; i < numProc; i++) {
		memCho << usedMemory[i].endAddress + 1 << "\n"
			"P" << usedMemory[i].processId << "\n"
			<< usedMemory[i].startAddress << "\n\n";
	}

	memCho << "----start---- = 0\n";
	return memCho.str();
}

void FlatMemoryAllocator::initializeMemory()
{
	freeMemory.push_back(MemoryBlock(0, maximumSize - 1, -1));
}