#include <iostream>
#include <cassert>
#include "HeapManager.h"

HeapManager::HeapManager(void* heapMemory, size_t heapSize, unsigned int numDescriptors) {
    //Later on I'd like to dynamically allocate blocks as part of the memory 
    //that gets allocated (allocate a block when you allocate memory) but that sounds too complicated for the moment.
    void* blockMemory = heapMemory;
    size_t blockMemorySize = sizeof(MemoryBlock) * numDescriptors;       

    initMemoryBlocks(blockMemory, blockMemorySize);

    freeBlocks = getFreeMemoryBlock();
    freeBlocks->baseAddress = static_cast<MemoryBlock*>(heapMemory) + numDescriptors; //Move the pointer forward however many memory blocks we're given
    freeBlocks->blockSize = heapSize - (blockMemorySize); //Shrink available memory by the number of blocks times the size of a block
    freeBlocks->nextBlock = nullptr;

    MemoryBlock* outstandingBlocks = nullptr;
}

void HeapManager::initMemoryBlocks(void* blocksMemory, size_t blocksMemorySize) {
    assert((blocksMemory != nullptr) && (blocksMemorySize > sizeof(MemoryBlock)));

    //Cast memory to MemoryBlocks
    blockList = static_cast<MemoryBlock*>(blocksMemory);
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

HeapManager::MemoryBlock* HeapManager::findFirstFittingFreeBlock(size_t i_size) const {
    MemoryBlock* freeBlock = freeBlocks;
    while (freeBlock) {
        if (freeBlock->blockSize >= i_size)
            break;
        freeBlock = freeBlock->nextBlock;
    }
    return freeBlock;
}

int HeapManager::getListSize(MemoryBlock* list) const {
    MemoryBlock* curBlock = list;
    short counter = 0;
    while (curBlock != nullptr) {
        counter++;
        curBlock = curBlock->nextBlock;
    }
    return counter;
}

bool HeapManager::detectLoop(MemoryBlock* list) const {
    //DEBUG ONLY!!!
    MemoryBlock* curBlock = list;
    short counter = 0;
    while (curBlock != nullptr) {
        counter++;
        if (counter > 2049) {
            return true;
        }
        curBlock = curBlock->nextBlock;
    }
    return false;
}

void HeapManager::printBlock(const MemoryBlock* block) const
{
    std::cout << "Pointer: " << block << ", BaseAddress: " << block->baseAddress << ", Size: " << block->blockSize;
    std::cout << ", Next: " << block->nextBlock << std::endl;
}

void HeapManager::Collect() {
    //std::cout << "Loop: " << detectLoop(freeBlocks) << "\n";

    MemoryBlock* curBlock = freeBlocks;
    MemoryBlock* next = curBlock->nextBlock;

    //While we still have a next block to coalesce with
    while (curBlock->nextBlock) {
        //cast to uint8_t* to allow for arithmetic, check if the blocks are adjacent
        if (static_cast<uint8_t*>(curBlock->baseAddress) + curBlock->blockSize == static_cast<uint8_t*>(next->baseAddress)) {
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

void HeapManager::removeFromFreeList(MemoryBlock* emptyFreeBlock) {
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
    //Alignment. Later on the option of allocating from the back of the block is interesting, but for now I'm doing the easiest option
    //Just round up to the nearest alignment value
    //TODO: Make sure the heap is 4-byte aligned to start
    size_t remainder = i_size % alignment;
    if (remainder != 0) {
        i_size += (alignment - remainder);
    }

    //Check the free list for a memory block of the right size
    MemoryBlock* freeBlock = findFirstFittingFreeBlock(i_size);
    if (freeBlock == nullptr) {
        Collect(); //With how I set up free() this should only ever need one time.
        freeBlock = findFirstFittingFreeBlock(i_size);
    }
    //I should only ever need to collect once, so we check again and if it's not working then we can't fulfill the request
    if (freeBlock == nullptr) {
        //std::cout << "returning nullptr\n";
        return nullptr;
    }

    MemoryBlock* block = getFreeMemoryBlock();

    //Check if there are no more free memory blocks (I'd like to change this later to create a memory block during each allocation)
    if (block == nullptr) {
        //We can't fulfill the allocation, return nullptr
        return nullptr;
    }

    block->baseAddress = freeBlock->baseAddress;
    block->blockSize = i_size;

    //std::cout << "Block: " << block->baseAddress << ", Size: " << block->blockSize << "\n";

    trackAlloc(block);

    //shrink the memory block
    //casting to a uint8_t* to allow for pointer arithmetic in increments of 1 byte
    freeBlock->baseAddress = static_cast<uint8_t*>(freeBlock->baseAddress) + i_size;
    freeBlock->blockSize -= i_size;
    if (freeBlock->blockSize == 0) {
        removeFromFreeList(freeBlock);
    }

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

void HeapManager::insertFreedBlock(MemoryBlock* block) {
    void* blockAddress = block->baseAddress;

    MemoryBlock* curBlock = freeBlocks;
    MemoryBlock* next = curBlock->nextBlock;
    void* curAddress = curBlock->baseAddress;
    void* nextAddress = nullptr;
    if (next != nullptr) {
        nextAddress = next->baseAddress;
    }

    //initial check to see if this should be the very first block
    if (blockAddress < curAddress) {
        //insert block at head of list
        block->nextBlock = freeBlocks;
        freeBlocks = block;
        return;
    }

    //While we still have a block to insert after (we need to have the current block and the next block so we can fill in the chain properly)
    while (curBlock) {
        if (blockAddress > curAddress) {
            //Make sure we have a next block
            if (next != nullptr) {
                if (blockAddress < nextAddress) {
                    //It's between current and next, so it should be inserted here and we can end
                    curBlock->nextBlock = block;
                    block->nextBlock = next;
                    return;
                }
                // else keep going, it's greater than both.
            }
            else {
                //if there's not a next block then this should be inserted at the end
                curBlock->nextBlock = block;
                block->nextBlock = nullptr;
                return;
            }
        }
        if (next == nullptr) {
            //if there's not a next block then this should be inserted at the end
            curBlock->nextBlock = block;
            block->nextBlock = nullptr;
            return;
        }

        curBlock = next;
        next = next->nextBlock;
        curAddress = curBlock->baseAddress;
        if (next != nullptr) {
            nextAddress = next->baseAddress;
        }
        else {
            nextAddress = nullptr;
        }
    }
}

void HeapManager::freeBlock(void* i_ptr) {
    MemoryBlock* block = removeOutstandingBlock(i_ptr);
    assert(block != nullptr);

    //As long as I put things in the right order I shouldn't need to coalesce more than once
    insertFreedBlock(block);
    //I'm collecting here to hopefully make certain debugging efforts easier. Also it's a bit more thorough.
    Collect();
}

bool HeapManager::contains(const void* i_ptr) const {
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

bool HeapManager::isAllocated(const void* i_ptr) const {
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

void HeapManager::showFreeBlocks() const
{
    MemoryBlock* curBlock = freeBlocks;
    while (curBlock != nullptr) {
        printBlock(curBlock);
        curBlock = curBlock->nextBlock;
    }
}

void HeapManager::showOutstandingBlocks() const
{
    MemoryBlock* curBlock = outstandingBlocks;
    while (curBlock != nullptr) {
        printBlock(curBlock);
        curBlock = curBlock->nextBlock;
    }
}
