#include <iostream>
#include <cassert>

inline void HeapManager::trackAlloc(MemoryBlock* block) {
    block->nextBlock = outstandingBlocks;
    outstandingBlocks = block;
}

inline HeapManager::MemoryBlock* HeapManager::getFreeMemoryBlock() {
    if (blockList == nullptr) {
        return nullptr;
    }

    MemoryBlock* returnBlock = blockList;
    blockList = blockList->nextBlock;

    return returnBlock;
}

inline void HeapManager::returnMemoryBlock(MemoryBlock* block) {
    assert(block != nullptr);

    block->baseAddress = nullptr;
    block->blockSize = 0;
    block->nextBlock = blockList;

    blockList = block;
}