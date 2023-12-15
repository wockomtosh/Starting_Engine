#pragma once

class HeapManager 
{
	struct MemoryBlock {
		void* baseAddress;
		size_t blockSize;
		MemoryBlock* nextBlock;
	};

	MemoryBlock* freeBlocks;
	MemoryBlock* outstandingBlocks;
	MemoryBlock* blockList;


	void initMemoryBlocks(void* blocksMemory, size_t blocksMemorySize);
	
	MemoryBlock* findFirstFittingFreeBlock(size_t i_size) const;
	int getListSize(MemoryBlock* list) const;
	
	bool detectLoop(MemoryBlock* list) const;
	void printBlock(const MemoryBlock* block) const;
	void removeFromFreeList(MemoryBlock* emptyFreeBlock);
	MemoryBlock* removeOutstandingBlock(void* i_ptr);
	void insertFreedBlock(MemoryBlock* block);
	inline void trackAlloc(MemoryBlock* block);
	inline MemoryBlock* getFreeMemoryBlock();
	inline void returnMemoryBlock(MemoryBlock* block);


public:
	HeapManager(void* heapMemory, size_t heapSize, unsigned int numDescriptors);

	void Collect();
	void* alloc(size_t i_size, unsigned int alignment = 4);
	void freeBlock(void* i_ptr);
	bool contains(const void* i_ptr) const;
	bool isAllocated(const void* i_ptr) const;
	void showFreeBlocks() const;
	void showOutstandingBlocks() const;
};

#include "HeapManager-inl.h"