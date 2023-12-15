#include "MemorySystem.h"


HeapManager* defaultHeapManager;
FixedSizeAllocator** FSAList;


bool InitializeMemorySystem(void * i_pHeapMemory, size_t i_sizeHeapMemory, unsigned int i_OptionalNumDescriptors)
{
	//Create HeapManager (Set aside the memory for it then call the constructor using placement new)
	defaultHeapManager = static_cast<HeapManager*>(i_pHeapMemory);
	i_pHeapMemory = static_cast<uint8_t*>(i_pHeapMemory) + sizeof(HeapManager);
	new (defaultHeapManager) HeapManager(i_pHeapMemory, i_sizeHeapMemory, i_OptionalNumDescriptors);

	FSAList = static_cast<FixedSizeAllocator**>(defaultHeapManager->alloc(sizeof(FixedSizeAllocator**) * numFSAs));

	//For each FSA, allocate the appropriate amount of space and also creat a bit array for each
	for (int i = 0; i < numFSAs; i++)
	{
		//Create the bit array to pass in
		//Round our numBlocks to the nearest 64 to make certain things easier
		size_t roundedNumBlocks = FSASizes[i].numBlocks + (64 - (FSASizes[i].numBlocks % 64));
		BitArray* bits = static_cast<BitArray*>(defaultHeapManager->alloc(sizeof(BitArray)));
		new (bits) BitArray(roundedNumBlocks, false);

		FixedSizeAllocator* FSA = static_cast<FixedSizeAllocator*>(defaultHeapManager->alloc(sizeof(FixedSizeAllocator)));
		void* FSAMemory = defaultHeapManager->alloc(FSASizes[i].blockSize * roundedNumBlocks);
		new (FSA) FixedSizeAllocator(FSAMemory, FSASizes[i].blockSize, roundedNumBlocks, bits);

		FSAList[i] = FSA;
	}

	return true;
}

void Collect()
{
	defaultHeapManager->Collect();
}

void DestroyMemorySystem()
{
	//Free the FSAs first using the HeapManager
	for (int i = 0; i < numFSAs; i++)
	{
		//Make sure to call the destructor. That will free the internal memory and the BitArray
		FSAList[i]->~FixedSizeAllocator();
		defaultHeapManager->freeBlock(FSAList[i]->getBaseAddress());
		defaultHeapManager->freeBlock(FSAList[i]);
	}
	defaultHeapManager->freeBlock(FSAList);
	FSAList = nullptr;

	defaultHeapManager = nullptr;
}

