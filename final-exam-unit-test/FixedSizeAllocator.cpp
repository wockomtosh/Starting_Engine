#include "FixedSizeAllocator.h"
#include <stdio.h>

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
		printf("ERROR: Outstanding allocations left when destroying allocator with size %zu\n", blockSize);
		blockTracker->SetAllBits();
	}
#endif

	//Destroy the BitArray. The internal memory will be freed in MemorySystem because we need to make sure it goes to the defaultHeapManager
	delete blockTracker;
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
		printf("ERROR: Trying to free a pointer that doesn't belong to the allocator with size %zu\n", blockSize);
	}
#endif

	//Just set the bit for this block to be available
	size_t blockNum = getBlockNum(i_ptr);
	
#ifdef _DEBUG
	if (blockTracker->IsBitSet(blockNum))
	{
		printf("ERROR: Trying to free a pointer that isn't allocated in the allocator with size %zu\n", blockSize);
	}
#endif

	blockTracker->SetBit(blockNum);
}

bool FixedSizeAllocator::contains(const void* i_ptr) const
{
	//Using a int64 since that will be compatible across systems, and I need to account for negative values.
	int64_t distanceFromBase = static_cast<uint8_t*>(const_cast<void*>(i_ptr)) - static_cast<uint8_t*>(baseAddress);

	//Check if it's in our address space
	if ((distanceFromBase >= 0) && (distanceFromBase < numBlocks * blockSize))
	{
		return true;
	}

	return false;
}

void* FixedSizeAllocator::getBaseAddress()
{
	return baseAddress;
}

size_t FixedSizeAllocator::getBlockNum(const void* i_ptr) const
{
	//Find the block it belongs to by subtracting baseAddress, then dividing by block size. 
	size_t distanceFromBase = static_cast<uint8_t*>(const_cast<void*>(i_ptr)) - static_cast<uint8_t*>(baseAddress);

#ifdef _DEBUG
	//To check validity use a mod operator to make sure it's a multiple of the block size after subtracting baseAddress
	if (distanceFromBase % blockSize != 0)
	{
		printf("ERROR: The given address does not align with the blocks in the allocator with size %zu\n", blockSize);
	}
#endif

	size_t blockNum = distanceFromBase / blockSize;

	return blockNum;
}
