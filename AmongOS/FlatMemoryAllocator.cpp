#include "FlatMemoryAllocator.h"
#include <sstream>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <filesystem>

#include "GlobalScheduler.h"

FlatMemoryAllocator::FlatMemoryAllocator(size_t maximumSize)
	: maximumSize(maximumSize)
{
	initializeMemory();

	std::string directory = "memory_files";
	std::filesystem::create_directory(directory);

	std::string filePath = directory + "/backing_store.txt";
	std::ofstream backingStore(filePath, std::ios::out | std::ios::trunc);
	backingStore.close();
}

FlatMemoryAllocator::~FlatMemoryAllocator()
{
}

void FlatMemoryAllocator::mergeFree()
{
	if (freeMemory.size() <= 1) {
		return;
	}

	freeMemory.sort([](const MemoryBlock& a, const MemoryBlock& b) {
		return a.startAddress < b.startAddress;
		});

	auto it = freeMemory.begin();
	while (it != freeMemory.end()) {
		auto next = std::next(it);
		if (next != freeMemory.end() && it->endAddress + 1 == next->startAddress) {
			it->endAddress = next->endAddress;
			freeMemory.erase(next);
		}
		else {
			++it;
		}
	}
}

bool FlatMemoryAllocator::allocate(std::shared_ptr<Process> p)
{
	mergeFree();

	size_t size = p->getSize();
	int pid = p->getPID();
	String processName = p->getName();

	// if process is already in memory, return true
	for (const auto& block : usedMemory) {
		if (block.processId == pid) {
			return true;
		}
	}

	// if process is too big for memory, return false
	if (size > maximumSize) {
		return false;
	}

	//allocate logic
	bool allocated = false;
	while (!allocated) {
		for (auto it = freeMemory.begin(); it != freeMemory.end(); ++it) {
			size_t blockSize = it->getSize();

			// if block is big enough for process, allocate
			if (size <= blockSize) {
				size_t startAddress = it->startAddress;
				size_t endAddress = startAddress + size - 1;

				usedMemory.emplace_back(startAddress, endAddress, pid, processName);
				if (blockSize == size) {
					freeMemory.erase(it);
				}
				else {
					it->startAddress = endAddress + 1;
				}

				usedMemory.sort([](const MemoryBlock& a, const MemoryBlock& b) {
					return a.startAddress < b.startAddress;
				});

				removeFromBackingStore(pid);
				allocated = true;
				break;
			}
		}

		// if process cannot be allocated, remove oldest block and try again
		if (!allocated) {
			if (usedMemory.size() >= 1) {
				if(!removeOldestBlock())
				{
					return false;
				}
			}

			if (usedMemory.empty() || freeMemory.empty()) {
				return false;
			}
		}
	}
}

bool FlatMemoryAllocator::removeOldestBlock()
{
	auto oldestBlockIt = std::min_element(usedMemory.begin(), usedMemory.end(), [](const MemoryBlock& a, const MemoryBlock& b) {
		return a.allocationTime < b.allocationTime;
	});

	while (oldestBlockIt != usedMemory.end()) {
		if (!GlobalScheduler::getInstance()->isProcessRunning(oldestBlockIt->processName)) {
			String content = std::to_string(oldestBlockIt->processId) + ", " +
				oldestBlockIt->processName + ", " +
				std::to_string(oldestBlockIt->getSize()) + "KB\n";

			freeMemory.push_back(*oldestBlockIt);
			usedMemory.erase(oldestBlockIt);
			mergeFree();

			writeToBackingStore(content);

			return true;
		}
		oldestBlockIt = std::next(oldestBlockIt);
	}

	return false;
}

void FlatMemoryAllocator::writeToBackingStore(String content)
{
	String directory = "memory_files";
	String filePath = directory + "/backing_store.txt";
	std::fstream backingStore(filePath, std::ios::in | std::ios::out);

	String line;
	bool written = false;

	// Find first empty line and write there
	while (std::getline(backingStore, line)) {
		if (line.empty()) {
			auto pos = backingStore.tellg();
			backingStore.seekp(pos - std::streamoff(line.size() + 1));
			backingStore << content;
			written = true;
			break;
		}
	}

	// If no empty line, write to end
	if (!written) {
		backingStore.clear();
		backingStore.seekp(0, std::ios::end);
		backingStore << content;
	}

	backingStore.close();
}

void FlatMemoryAllocator::removeFromBackingStore(int pid)
{
	String directory = "memory_files";
	String filePath = directory + "/backing_store.txt";
	std::fstream backingStore(filePath, std::ios::in | std::ios::out);

	std::stringstream buffer;
	String line;

	while (std::getline(backingStore, line)) {
		if (line.substr(0, line.find(',')) != std::to_string(pid)) {
			buffer << line << "\n"; 
		}
	}

	backingStore.close();

	backingStore.open(filePath, std::ios::out | std::ios::trunc);
	backingStore << buffer.str();
	backingStore.close();
}

void FlatMemoryAllocator::deallocate(int pid)
{
	auto it = usedMemory.begin();
	while (it != usedMemory.end()) {
		if (it->processId == pid) {
			freeMemory.push_back(*it);
			it = usedMemory.erase(it);
		}
		else {
			++it;
		}
	}

	removeFromBackingStore(pid);

	mergeFree();
}

String FlatMemoryAllocator::visualizeProcessesInMemory()
{	//old code to be removed

	//std::ostringstream memCho;

	//auto now = std::chrono::system_clock::now();
	//std::time_t timeStamp = std::chrono::system_clock::to_time_t(now);

	//std::tm timeInfo;
	//localtime_s(&timeInfo, &timeStamp);

	//size_t externFrag = 0;
	//mergeFree();

	//for (const auto& block : freeMemory) {
	//	if (block.endAddress == maximumSize - 1) {
	//		continue;
	//	}
	//	externFrag += block.getSize();
	//}

	//int numProc = static_cast<int>(usedMemory.size());

	//memCho << "Timestamp: " << std::put_time(&timeInfo, "(%m/%d/%Y %I:%M:%S%p)") << "\n";
	//memCho << "Number of processes in memory: " << numProc << "\n";
	//memCho << "Total external fragmentation in KB: " << externFrag << "\n\n";

	//memCho << "----end---- = " << maximumSize << "\n\n";

	//for (const auto& block : usedMemory) {
	//	memCho << block.endAddress + 1 << "\n"
	//		<< "P" << block.processId << ", " << block.processName << "\n"
	//		<< block.startAddress << "\n\n";
	//}

	//memCho << "----start---- = 0\n";
	//return memCho.str();

	size_t usedMemorySize = 0;
	size_t freeMemorySize = 0;

	// Calculate total used memory
	for (const auto& block : usedMemory) {
		usedMemorySize += block.getSize();
	}

	// Calculate total free memory
	for (const auto& block : freeMemory) {
		freeMemorySize += block.getSize();
	}
	size_t totalMemory = maximumSize;
	float memoryUtilization = (static_cast<float>(usedMemorySize) / totalMemory) * 100;
	std::ostringstream visualizedMemory;

	visualizedMemory << "\n";
	visualizedMemory << "--------------------------------------------" << "\n";
	visualizedMemory << "| PROCESS-SMI V01.00 Driver Version: 01.00 |" << "\n";
	visualizedMemory << "--------------------------------------------" << "\n";
	visualizedMemory << "CPU utilization: " << GlobalScheduler::getInstance()->getCPUUtilization() << "%" << "\n";
	visualizedMemory << "Memory Usage: "<< usedMemorySize << "KB / " << totalMemory << "KB\n";
	visualizedMemory << "Memory Utilization: " << std::fixed << std::setprecision(0) << memoryUtilization << "%\n";
	visualizedMemory << "\n";
	visualizedMemory << "============================================" << "\n";
	visualizedMemory << "Running Processes and memory usage: " << "\n";
	visualizedMemory << "--------------------------------------------" << "\n";
	for (const auto& block : usedMemory) {
		visualizedMemory << block.processName << "  ";
		visualizedMemory << block.getSize() << "KB\n";
	}
	visualizedMemory << "--------------------------------------------" << "\n";
	return visualizedMemory.str();
}

void FlatMemoryAllocator::initializeMemory()
{
	freeMemory.push_back(MemoryBlock(0, maximumSize - 1, -1, ""));
}