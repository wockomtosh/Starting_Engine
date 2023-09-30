#include <iostream>
#include <cassert>

class HeapManager {
    struct MemoryBlock {
        void* baseAddress;
        size_t blockSize;
        struct MemoryBlock* nextBlock;
    };

    MemoryBlock* heap;
    MemoryBlock* freeBlocks;
    MemoryBlock* outstandingBlocks;

    //The list of actual memory block structures
    MemoryBlock* blockList;

    void initHeapMemory() {

    }

    void initMemoryBlocks(void* blocksMemory, size_t blocksMemorySize) {
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

    void initMem() {
        //Initialize the heap



        //Initialize list of MemoryBlocks. I need to allocate some amount of memory to do this.
        initMemoryBlocks(nullptr, 0);

        //Get free and outstanding block lists
        MemoryBlock* freeBlocks = heap;
        MemoryBlock* outstandingBlocks = nullptr;
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

    MemoryBlock* getFreeMemoryBlock() {
        //Get a free memory block struct from the list that was initialized at the beginning
        assert(blockList != nullptr);

        MemoryBlock* returnBlock = blockList;
        blockList = blockList->nextBlock;

        return returnBlock;
    }

    void returnMemoryBlock(MemoryBlock* block) {
        assert(block != nullptr);

        block->baseAddress = nullptr;
        block->blockSize = 0;
        block->nextBlock = blockList;

        blockList = block;
    }

    void Collect() {

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

    void* alloc(size_t i_size) {
        //Check the free list for a memory block of the right size
        MemoryBlock* freeBlock = findFirstFittingFreeBlock(i_size);
        if (freeBlock == nullptr) {
            Collect(); //With how I set up free() this should only ever need one time.
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
};