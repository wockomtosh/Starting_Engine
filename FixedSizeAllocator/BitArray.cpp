#include "BitArray.h"
#include <vcruntime_string.h>
#include <intrin.h>
#include <iostream>

BitArray::BitArray(size_t i_numBits, bool initToZero)
{
    numBits = i_numBits;
    bits = new BitData[numBits / bitsPerElement];

    //Memset sets things in chunks of sizeof(int), so we need to make sure we account for that.
    //To set everything to 1 we need to make sure all bits in the int are set to 1
    std::cout << (numBits / bitsPerElement) << std::endl;
    std::cout << (bitsPerElement / (sizeof(int) * 8)) << std::endl;
    std::cout << (bitsPerElement) << std::endl;
    std::cout << (numBits / bitsPerElement) * (bitsPerElement / (sizeof(int) * 8));
    memset(bits, initToZero ? 0 : 0xFFFFFFFF, (numBits / bitsPerElement) * (bitsPerElement / (sizeof(int) * 8)));
}

BitArray::~BitArray()
{
}

bool BitArray::FindFirstSetBit(size_t& firstSetBitIndex) const
{
    size_t index = 0;
    size_t numElements = numBits / bitsPerElement;

    //Get the block of bits that contains the first set bit
    while ((bits[index] == BitData(0)) && (index < numElements))
    {
        index++;
    }

    //Check if there are no set bits
    if (index >= numElements) 
    {
        return false;
    }

    BitData currentBits = bits[index];
    unsigned long bitIndex;

    //Because BitScanForward indexes from the least significant bit, each set of 64 bits would be backwards from how you write them.
    //This shouldn't be an issue if we account for it when indexing
#if WIN32
    _BitScanForward(bitIndex, currentBits);
#else
    _BitScanForward64(&bitIndex, currentBits);
#endif

    firstSetBitIndex = static_cast<size_t>(bitIndex);

    return true;
}

bool BitArray::FindFirstClearBit(size_t& firstClearBitIndex) const
{
    size_t index = 0;
    size_t numElements = numBits / bitsPerElement;

    //Get the block of bits that contains the first set bit
    while ((bits[index] == BitData(0)) && (index < numElements))
    {
        index++;
    }

    //Check if there are no set bits
    if (index >= numElements)
    {
        return false;
    }

    BitData currentBits = bits[index];
    unsigned long bitIndex;

    //Because BitScanForward indexes from the least significant bit, each set of 64 bits would be backwards from how you write them.
    //This shouldn't be an issue if we account for it when indexing
    //The only difference between the first set bit and first clear bit is that we can do a bitwise NOT and then git the first set bit.
#if WIN32
    _BitScanForward(bitIndex, ~currentBits);
#else
    _BitScanForward64(&bitIndex, ~currentBits);
#endif

    firstClearBitIndex = static_cast<size_t>(bitIndex);

    return true;
}

void BitArray::ClearAllBits()
{
    memset(bits, 0, (numBits / bitsPerElement) * (bitsPerElement / (sizeof(int) * 8)));
}

void BitArray::SetAllBits()
{
    memset(bits, 0xFFFFFFFF, (numBits / bitsPerElement) * (bitsPerElement / (sizeof(int) * 8)));
}

bool BitArray::AreAllBitsClear() const
{
    size_t index = 0;
    size_t numElements = numBits / bitsPerElement;

    //Iterate through until you find any nonzero bit
    while ((bits[index] == BitData(0)) && (index < numElements))
    {
        index++;
    }

    //If you got through the whole array then everything is clear
    if (index >= numElements)
    {
        return true;
    }

    return false;
}

bool BitArray::AreAllBitsSet() const
{
    size_t index = 0;
    size_t numElements = numBits / bitsPerElement;

    while ((bits[index] == BitData(1)) && (index < numElements))
    {
        index++;
    }

    if (index >= numElements)
    {
        return true;
    }

    return false;
}

void BitArray::SetBit(size_t bitNumber)
{
    size_t elementIndex = bitNumber / bitsPerElement;
    size_t bitIndex = bitNumber % bitsPerElement;

    BitData element = bits[elementIndex];

    std::cout << "Element: " << element << " mask: " << (static_cast<BitData>(1) << bitIndex);

    //Take the element, and OR it with a mask that has a 1 set on our desired bit
    bits[elementIndex] = element | (static_cast<BitData>(1) << bitIndex);
}

void BitArray::ClearBit(size_t bitNumber)
{
    size_t elementIndex = bitNumber / bitsPerElement;
    size_t bitIndex = bitNumber % bitsPerElement;

    BitData element = bits[elementIndex];

    //Take the element, and AND it with a mask that has a 0 set on our desired bit and a 1 everywhere else
    bits[elementIndex] = element & ~(static_cast<BitData>(1) << bitIndex);
}

bool BitArray::operator[](size_t bitIndex) const
{
    if (IsBitSet(bitIndex))
    {
        return true;
    }
    return false;
}
