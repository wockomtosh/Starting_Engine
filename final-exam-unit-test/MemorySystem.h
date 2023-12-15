#pragma once
#include "HeapManager.h"
#include "FixedSizeAllocator.h"
#include "BitArray.h"

struct FSAInitData
{
	size_t blockSize;
	size_t numBlocks;
};

static const int numFSAs = 3;
static FSAInitData FSASizes[numFSAs] = { {16, 100}, {32, 200}, {96, 400} };

static HeapManager* defaultHeapManager;
static FixedSizeAllocator** FSAList;

// InitializeMemorySystem - initialize your memory system including your HeapManager and some FixedSizeAllocators
bool InitializeMemorySystem(void * i_pHeapMemory, size_t i_sizeHeapMemory, unsigned int i_OptionalNumDescriptors);

// Collect - coalesce free blocks in attempt to create larger blocks
void Collect();

// DestroyMemorySystem - destroy your memory systems
void DestroyMemorySystem();
