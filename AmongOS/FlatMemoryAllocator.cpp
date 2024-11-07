#include "FlatMemoryAllocator.h"
#include <sstream>

FlatMemoryAllocator::FlatMemoryAllocator(size_t maximumSize)
	: maximumSize(maximumSize), allocatedSize(0)
{
	memory.reserve(maximumSize);
	initializeMemory();
}

FlatMemoryAllocator::~FlatMemoryAllocator()
{
	memory.clear();
}

bool FlatMemoryAllocator::allocate(size_t size, int pid)
{
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

		return true;
	}
	
	//for (size_t i = 0; i < maximumSize - size + 1; i++)
	//{
	//	if (!allocationMap[i] && (i + size <= maximumSize))
	//	{
	//		//std::fill(memory.begin() + index, memory.begin() + index + size, 'X'); // 'X' is allocated memory

	//		for (size_t j = i; j < i + size; ++j) {
	//			if (allocationMap.find(j) != allocationMap.end()) {
	//				allocationMap[j] = true;
	//			}
	//		}

	//		allocatedSize += size;
	//		return &memory[i];
	//	}
	//}

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

	for (size_t i = 0; i < freeMemory.size() - 1; ++i) {
		if (freeMemory[i].endAddress + 1 == freeMemory[i + 1].startAddress) {
			freeMemory[i].endAddress = freeMemory[i + 1].endAddress;
			freeMemory.erase(freeMemory.begin() + i + 1);
			--i;
		}
	}

	//size_t index = static_cast<char*>(ptr) - &memory[0];
	//if(allocationMap[index])
	//{
	//	allocationMap[index] = false;

	//	/*std::fill(memory.begin() + index, memory.begin() + index + size, '.');
	//	std::fill(allocationMap.begin() + index, allocationMap.begin() + index + size, false);
	//	allocatedSize -= size;*/
	//}
}

String FlatMemoryAllocator::visualizeProcessesInMemory()
{
	std::ostringstream memCho;

	memCho << "----end---- = " << maximumSize << "\n";

	for (size_t i = 0; i < usedMemory.size(); i++) {
		memCho << "Process ID: " << usedMemory[i].processId
			<< ", Start: " << usedMemory[i].startAddress
			<< ", End: " << usedMemory[i].endAddress
			<< ", Size: " << usedMemory[i].getSize() << "\n";
	}

	memCho << "----start---- = 0\n";
	return memCho.str();
}

void FlatMemoryAllocator::initializeMemory()
{
	//std::fill(memory.begin(), memory.end(), '.'); // '.' is unallocated memory
	////::fill(allocationMap.begin(), allocationMap.end(), false);

	//for (size_t i = 0; i < maximumSize; ++i) {
	//	allocationMap[i] = false;
	//}

	freeMemory.push_back(MemoryBlock(0, maximumSize, -1));
}