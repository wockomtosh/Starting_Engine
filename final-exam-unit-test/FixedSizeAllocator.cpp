#include "FixedSizeAllocator.h"
#include <iostream>

FixedSizeAllocator::FixedSizeAllocator(void* memory, size_t size, unsigned int numBlocks, BitArray* bits)
	: baseAddress(memory), blockSize(size), numBlocks(numBlocks), blockTracker(bits)
{
}

FixedSizeAllocator::~FixedSizeAllocator()
{
#ifdef _DEBUG
	//Check to make sure there are no outstanding allocations (Just log something and free them?)
	if (!blockTracker->AreAllBitsSet())
	{
		std::cout << "ERROR: Outstanding allocations left when destroying allocator with size " << blockSize << std::endl;
		blockTracker->SetAllBits();
	}
#endif

	//Destroy the BitArray
	//How do I free it when I allocated the space for it? Do I just let that happen when I free the FSA in the memory system?
}

void* FixedSizeAllocator::alloc()
{
	size_t firstAvailable;

	if (blockTracker->FindFirstSetBit(firstAvailable))
	{
		blockTracker->ClearBit(firstAvailable);

		void* blockAddress = static_cast<uint8_t*>(baseAddress) + (firstAvailable * blockSize);

		return blockAddress;
	}
	else
		return nullptr;
}

void FixedSizeAllocator::freeBlock(void* i_ptr)
{
#ifdef _DEBUG
	if (!contains(i_ptr))
	{
		std::cout << "ERROR: Trying to free a pointer that doesn't belong to the allocator with size " << blockSize << std::endl;
	}
#endif

	//Just set the bit for this block to be available
	size_t blockNum = getBlockNum(i_ptr);
	
#ifdef _DEBUG
	if (blockTracker->IsBitSet(blockNum))
	{
		std::cout << "ERROR: Trying to free a pointer that isn't allocated in the allocator with size " << blockSize << std::endl;
	}
#endif

	blockTracker->SetBit(blockNum);
}

bool FixedSizeAllocator::contains(const void* i_ptr) const
{
	size_t distanceFromBase = static_cast<uint8_t*>(const_cast<void*>(i_ptr)) - static_cast<uint8_t*>(baseAddress);

	//Check if it's in our address space
	if ((distanceFromBase >= 0) && (distanceFromBase < numBlocks * blockSize))
	{
		return true;
	}

	return false;
}

size_t FixedSizeAllocator::getBlockNum(const void* i_ptr) const
{
	//Find the block it belongs to by subtracting baseAddress, then dividing by block size. 
	size_t distanceFromBase = static_cast<uint8_t*>(const_cast<void*>(i_ptr)) - static_cast<uint8_t*>(baseAddress);

#ifdef _DEBUG
	//To check validity use a mod operator to make sure it's a multiple of the block size after subtracting baseAddress
	if (distanceFromBase % blockSize != 0)
	{
		std::cout << "ERROR: The given address does not align with the blocks in the allocator with size " << blockSize << std::endl;
	}
#endif

	size_t blockNum = distanceFromBase / blockSize;

	return blockNum;
}
