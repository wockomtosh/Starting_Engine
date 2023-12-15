#pragma once
#include "BitArray.h"

class FixedSizeAllocator
{
public:
	FixedSizeAllocator(void* memory, size_t size, unsigned int numBlocks, BitArray* bits);
	~FixedSizeAllocator();

	void* alloc();
	void freeBlock(void* i_ptr);
	bool contains(const void* i_ptr) const;
	void* getBaseAddress();

private:
	BitArray* blockTracker;
	size_t blockSize;
	unsigned int numBlocks;
	void* baseAddress;

	size_t getBlockNum(const void* i_ptr) const;
};