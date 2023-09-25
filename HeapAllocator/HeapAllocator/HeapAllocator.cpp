#include <iostream>
#include <cassert>

struct MemoryBlock {
    void* baseAddress;
    size_t blockSize;
    struct MemoryBlock* nextBlock;
};

MemoryBlock* heap;
MemoryBlock* freeBlocks;
MemoryBlock* outstandingBlocks;

void initMem() {
    //Initialize the heap
    //Initialize list of MemoryBlocks
    

    //Get free and outstanding block lists
    MemoryBlock* freeBlocks = heap;
    MemoryBlock* outstandingBlocks = nullptr;
}

MemoryBlock* getFreeMemoryBlock() {
    //Get a free memory block struct from the list that was initialized at the beginning
}

void trackAlloc(MemoryBlock* block) {
    //Put the memory block into the outstanding list

    //Puts each outstanding block in front of the last
    block->nextBlock = outstandingBlocks;
    outstandingBlocks = block;
}

MemoryBlock* findFirstFittingFreeBlock(size_t i_size) {
    MemoryBlock* freeBlock = freeBlocks;
    while (freeBlock) {
        if (freeBlock->blockSize >= i_size)
            break;
        freeBlock = freeBlock->nextBlock;
    }
    return freeBlock;
}

void coalesceFreeBlocks() {
    //foreach pair of blocks
    //if (block1->baseAddress + block1->blockSize == block2->baseAddress) {
    //    block1->blockSize += block2->blockSize;
    //    returnMemoryBlock(block2);
    //}
    
    //Only move to the next block if you didn't combine two blocks. If you have 3 abutting blocks you want them all to be combined.
}

void* alloc(size_t i_size) {
    //Check the free list for a memory block of the right size
    MemoryBlock* freeBlock = findFirstFittingFreeBlock(i_size);
    if (freeBlock == nullptr) {
        coalesceFreeBlocks();
        freeBlock = findFirstFittingFreeBlock(i_size);
    }
    assert(freeBlock); 

    if (freeBlock->blockSize == i_size) {
        //Removefromfreelist()
        trackAlloc(freeBlock);
        return freeBlock->baseAddress;
    }
    else {
        MemoryBlock* block = getFreeMemoryBlock();

        block->baseAddress = freeBlock->baseAddress;
        block->blockSize = i_size;
        trackAlloc(block);

        //shrink the memory block
        //casting to a char* to allow for pointer arithmetic in increments of 1 byte
        freeBlock->baseAddress = (char*)freeBlock->baseAddress + i_size;
        freeBlock->blockSize -= i_size;

        return block->baseAddress;
    }
}

MemoryBlock* removeOutstandingBlock(void* i_ptr) {
    //Search for the i_ptr, remove it from the linked list and return
    bool isFound = false;
    MemoryBlock* currentBlock = outstandingBlocks;
    MemoryBlock* prevBlock = nullptr;

    while (!isFound) {
        //If we reach the end we want the loop to exit
        if (currentBlock == nullptr) {
            return currentBlock;
        }

        //If we find it then we want to remove and return it
        if (currentBlock->baseAddress == i_ptr) {
            //Remove the block
            if (prevBlock != nullptr) {
                prevBlock->nextBlock = currentBlock->nextBlock;
            }
            else {
                outstandingBlocks = currentBlock->nextBlock;
            }
            return currentBlock;
        }

        prevBlock = currentBlock;
        currentBlock = currentBlock->nextBlock;
    }
}

void* freeBlock(void* i_ptr) {
    MemoryBlock* block = removeOutstandingBlock(i_ptr);
    assert(block);

    //As long as I put things in the right order I shouldn't need to coalesce more than once
    //I can also coalesce on insertion into the free list if I want
    
    //TODO: Fix ordering
    block->nextBlock = freeBlocks;
    freeBlocks = block;    
}

int main()
{
    std::cout << "Starting HeapAllocator...\n";
}