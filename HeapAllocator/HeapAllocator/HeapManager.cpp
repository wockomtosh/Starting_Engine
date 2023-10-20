#include <iostream>
#include <cassert>
#include "HeapManager.h"

HeapManager::HeapManager(void* heapMemory, size_t heapSize, unsigned int numDescriptors) {
    //TODO: Make sure the heap is 4-byte aligned to start

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
    freeBlocks->nextBlock = nullptr;

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

int HeapManager::getListSize(MemoryBlock* list) {
    MemoryBlock* curBlock = list;
    int counter = 0;
    while (curBlock != nullptr) {
        counter++;
        curBlock = curBlock->nextBlock;
    }
    return counter;
}

HeapManager::MemoryBlock* HeapManager::getFreeMemoryBlock() {
    //Get a free memory block struct from the list that was initialized at the beginning
    if (blockList == nullptr) {
        return nullptr;
    }

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

    //std::cout << getListSize(blockList) << "\n";
}

bool HeapManager::detectLoop(MemoryBlock* list) {
    MemoryBlock* curBlock = list;
    int counter = 0;
    while (curBlock != nullptr) {
        counter++;
        if (counter > 2049) {
            return true;
        }
        curBlock = curBlock->nextBlock;
    }
    return false;
}

void HeapManager::Collect() {
    //std::cout << "Loop: " << detectLoop(freeBlocks) << "\n";

    MemoryBlock* curBlock = freeBlocks;
    MemoryBlock* next = curBlock->nextBlock;

    //While we still have a next block to coalesce with
    while (curBlock->nextBlock) {
        if (getListSize(blockList) == 2046) {
            std::cout << "break\n";
        }
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
    //casting to a char* to allow for pointer arithmetic in increments of 1 byte
    freeBlock->baseAddress = (char*)freeBlock->baseAddress + i_size;
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

void HeapManager::insertFreedBlock(HeapManager::MemoryBlock* block) {
    void* blockAddress = block->baseAddress;

    MemoryBlock* curBlock = freeBlocks;
    MemoryBlock* next = curBlock->nextBlock;
    void* curAddress = curBlock->baseAddress;
    void* nextAddress = 0x0;
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
    //if (block->blockSize == 564) {
    //    //we have our loop?
    //    std::cout << "break\n\n\n";
    //}
    //std::cout << "free\n";
    //std::cout << "Block: " << block->baseAddress << ", Size: " << block->blockSize << "\n";
    //std::cout << "Loop: " << detectLoop(freeBlocks) << "\n";

    //As long as I put things in the right order I shouldn't need to coalesce more than once
    insertFreedBlock(block);
    //I'm collecting here to hopefully make certain debugging efforts easier.
    //Collect();
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