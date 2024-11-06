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

void* FlatMemoryAllocator::allocate(size_t size)
{
	for (size_t i = 0; i < maximumSize - size + 1; i++)
	{
		if (!allocationMap[i] && canAllocateAt(i, size))
		{
			allocateAt(i, size);
			return &memory[i];
		}
	}

	return nullptr;
}

void FlatMemoryAllocator::deallocate(void* ptr)
{
	size_t index = static_cast<char*>(ptr) - &memory[0];
	if(allocationMap[index])
	{
		deallocateAt(index);
	}
}

String FlatMemoryAllocator::visualizeMemory()
{
	return String(memory.begin(), memory.end());
}

String FlatMemoryAllocator::visualizeProcessesInMemory()
{
	std::ostringstream displayStream;

	displayStream << "----end---- = " << maximumSize << "\n";

	displayStream << "----start---- = 0\n";

	return displayStream.str();
}

void FlatMemoryAllocator::initializeMemory()
{
	std::fill(memory.begin(), memory.end(), '.'); // '.' is unallocated memory
	//::fill(allocationMap.begin(), allocationMap.end(), false);

	for (size_t i = 0; i < maximumSize; ++i) {
		allocationMap[i] = false; 
	}
}

bool FlatMemoryAllocator::canAllocateAt(size_t index, size_t size) const
{
	return (index + size <= maximumSize);
}

void FlatMemoryAllocator::allocateAt(size_t index, size_t size)
{
	//std::fill(memory.begin() + index, memory.begin() + index + size, 'X'); // 'X' is allocated memory

	for (size_t i = index; i < index + size; ++i) {
		if (allocationMap.find(i) != allocationMap.end()) {
			allocationMap[i] = true;
		}
	}

	allocatedSize += size;
}

void FlatMemoryAllocator::deallocateAt(size_t index)
{
	allocationMap[index] = false;

	/*std::fill(memory.begin() + index, memory.begin() + index + size, '.');
	std::fill(allocationMap.begin() + index, allocationMap.begin() + index + size, false);
	allocatedSize -= size;*/
}