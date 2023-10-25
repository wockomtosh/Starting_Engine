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
	void trackAlloc(MemoryBlock* block);
	MemoryBlock* findFirstFittingFreeBlock(size_t i_size);
	MemoryBlock* getFreeMemoryBlock();
	void returnMemoryBlock(MemoryBlock* block);
	void removeFromFreeList(MemoryBlock* emptyFreeBlock);
	MemoryBlock* removeOutstandingBlock(void* i_ptr);
	void insertFreedBlock(MemoryBlock* block);
	int getListSize(MemoryBlock* list);
	bool detectLoop(MemoryBlock* list);
	void printBlock(MemoryBlock* block) const;


public:
	HeapManager(void* heapMemory, size_t heapSize, unsigned int numDescriptors);

	void Collect();
	void* alloc(size_t i_size, unsigned int alignment = 4);
	void freeBlock(void* i_ptr);
	bool contains(void* i_ptr) const;
	bool isAllocated(void* i_ptr) const;
	void showFreeBlocks() const;
	void showOutstandingBlocks() const;
};