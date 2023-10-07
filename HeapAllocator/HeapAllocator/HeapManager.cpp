#include <iostream>
#include <cassert>
#include "HeapManager.h"

HeapManager::HeapManager(void* heapMemory, size_t heapSize, unsigned int numDescriptors) {
    //Have a reference to the heap
    //TODO: Make sure the heap is 4-byte aligned to start
    //heap->baseAddress = heapMemory;
    //heap->blockSize = heapSize;

    //Split off a chunk of the heap for memory blocks
    void* blockMemory = heapMemory;
    size_t blockMemorySize = sizeof(MemoryBlock) * numDescriptors;       

    //Get the actual block structures. Later on I'd like to dynamically allocate blocks as part of the memory 
    //that gets allocated (allocate a block when you allocate memory) but that sounds too complicated for the moment.
    initMemoryBlocks(blockMemory, blockMemorySize);

    //Set up freeBlocks
    freeBlocks = getFreeMemoryBlock();
    freeBlocks->baseAddress = (MemoryBlock*)heapMemory + numDescriptors; //Move the pointer forward however many memory blocks we're given
    freeBlocks->blockSize = heapSize - (blockMemorySize); //Shrink available memory by the number of blocks times the size of a block

    //Get outstanding blocks (starts as just a null)
    MemoryBlock* outstandingBlocks = nullptr;
}

void HeapManager::initMemoryBlocks(void* blocksMemory, size_t blocksMemorySize) {
    assert((blocksMemory != nullptr) && (blocksMemorySize > sizeof(MemoryBlock)));

    //Set up memory as being a pointer to MemoryBlocks
    blockList = (MemoryBlock*)blocksMemory;
    //Get the number of blocks based on available memory
    const size_t numBlocks = blocksMemorySize / sizeof(MemoryBlock);

    MemoryBlock* curBlock = blockList;
    for (size_t i = 0; i < (numBlocks - 1); i++, curBlock++) {
        curBlock->baseAddress = nullptr;
        curBlock->blockSize = 0;
        curBlock->nextBlock = curBlock + 1;
    }
    //final block
    curBlock->baseAddress = nullptr;
    curBlock->blockSize = 0;
    curBlock->nextBlock = nullptr;
}

void HeapManager::trackAlloc(MemoryBlock* block) {
    //Put the memory block into the outstanding list

    //Puts each outstanding block in front of the last
    block->nextBlock = outstandingBlocks;
    outstandingBlocks = block;
}

HeapManager::MemoryBlock* HeapManager::findFirstFittingFreeBlock(size_t i_size) {
    MemoryBlock* freeBlock = freeBlocks;
    while (freeBlock) {
        if (freeBlock->blockSize >= i_size)
            break;
        freeBlock = freeBlock->nextBlock;
    }
    return freeBlock;
}

HeapManager::MemoryBlock* HeapManager::getFreeMemoryBlock() {
    //Get a free memory block struct from the list that was initialized at the beginning
    assert(blockList != nullptr);

    MemoryBlock* returnBlock = blockList;
    blockList = blockList->nextBlock;

    return returnBlock;
}

void HeapManager::returnMemoryBlock(MemoryBlock* block) {
    assert(block != nullptr);

    block->baseAddress = nullptr;
    block->blockSize = 0;
    block->nextBlock = blockList;

    blockList = block;
}

void HeapManager::Collect() {

    MemoryBlock* curBlock = freeBlocks;
    MemoryBlock* next = curBlock->nextBlock;

    //While we still have a next block to coalesce with
    while (curBlock->nextBlock) {
        //convert to char* to allow for arithmetic, check if the blocks are adjacent
        if ((char*)curBlock->baseAddress + curBlock->blockSize == (char*)next->baseAddress) {
            curBlock->blockSize += next->blockSize;
            curBlock->nextBlock = next->nextBlock;
            returnMemoryBlock(next);

            //Only move curBlock to the next block if you didn't combine two blocks. If you have more adjacent blocks you want them all to be combined
            next = curBlock->nextBlock;
        }
        else {
            curBlock = next;
            next = next->nextBlock;
        }
    }

}

void HeapManager::removeFromFreeList(HeapManager::MemoryBlock* emptyFreeBlock) {
    //We need the previous block in the list to point to the one after the block we're trying to remove
    MemoryBlock* prevBlock = freeBlocks;
    while (prevBlock->nextBlock != nullptr) {
        if (prevBlock->nextBlock == emptyFreeBlock) {
            prevBlock->nextBlock = emptyFreeBlock->nextBlock;
            //Once we've got it we can return that empty block
            returnMemoryBlock(emptyFreeBlock);
            break;
        }
        prevBlock = prevBlock->nextBlock;
    }
}

void* HeapManager::alloc(size_t i_size, unsigned int alignment) {
    //Check the free list for a memory block of the right size
    MemoryBlock* freeBlock = findFirstFittingFreeBlock(i_size);
    if (freeBlock == nullptr) {
        Collect(); //With how I set up free() this should only ever need one time.
        freeBlock = findFirstFittingFreeBlock(i_size);
    }
    //I should only ever need to collect once, so we check again and if it's not working then we can't fulfill the request
    if (freeBlock == nullptr) {
        std::cout << "returning nullptr\n";
        return nullptr;
    }

    //Alignment. Later on the option of allocating from the back of the block is interesting, but for now I'm doing the easiest option
    //Just round up to the nearest alignment value
    //TODO: Make sure the heap is 4-byte aligned to start
    size_t remainder = i_size % alignment;
    if (remainder != 0) {
        i_size += (alignment - remainder);
    }


    MemoryBlock* block = getFreeMemoryBlock();

    block->baseAddress = freeBlock->baseAddress;
    block->blockSize = i_size;
    trackAlloc(block);

    //shrink the memory block
    //casting to a char* to allow for pointer arithmetic in increments of 1 byte
    freeBlock->baseAddress = (char*)freeBlock->baseAddress + i_size;
    freeBlock->blockSize -= i_size;
    if (freeBlock->blockSize == 0) {
        removeFromFreeList(freeBlock);
    }

    std::cout << block->baseAddress << "\n";
    return block->baseAddress;
}

HeapManager::MemoryBlock* HeapManager::removeOutstandingBlock(void* i_ptr) {
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

void HeapManager::freeBlock(void* i_ptr) {
    MemoryBlock* block = removeOutstandingBlock(i_ptr);
    assert(block);

    //As long as I put things in the right order I shouldn't need to coalesce more than once
    //I can also coalesce on insertion into the free list if I want, but I think coalescing in alloc works fine

    //TODO: Fix ordering
    block->nextBlock = freeBlocks;
    freeBlocks = block;
}

bool HeapManager::contains(void* i_ptr) const {
    //loop through blockList, freeBlocks, and outstandingBlocks to see if this pointer matches any baseAddress
    MemoryBlock* curBlock = outstandingBlocks;
    while (curBlock != nullptr) {
        if (curBlock->baseAddress == i_ptr) {
            return true;
        }
        curBlock = curBlock->nextBlock;
    }

    curBlock = freeBlocks;
    while (curBlock != nullptr) {
        if (curBlock->baseAddress == i_ptr) {
            return true;
        }
        curBlock = curBlock->nextBlock;
    }

    curBlock = blockList;
    while (curBlock != nullptr) {
        if (curBlock->baseAddress == i_ptr) {
            return true;
        }
        curBlock = curBlock->nextBlock;
    }

    return false;
}

bool HeapManager::isAllocated(void* i_ptr) const {
    //loop through outstandingBlocks to see if this pointer matches any baseAddress
    MemoryBlock* curBlock = outstandingBlocks;
    while (curBlock != nullptr) {
        if (curBlock->baseAddress == i_ptr) {
            return true;
        }
        curBlock = curBlock->nextBlock;
    }
    return false;
}