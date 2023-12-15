#include "BitArray.h"
#include <vcruntime_string.h>
#include <intrin.h>
#include <iostream>

BitArray::BitArray(size_t i_numBits, bool initToZero)
{
    numBits = i_numBits;
    size_t bitsPerElement = sizeof(BitData) * 8;
    bits = new BitData[numBits / bitsPerElement];

    if (initToZero)
    {
        ClearAllBits();
    }
    else
    {
        SetAllBits();
    }
}

BitArray::~BitArray()
{
    delete[] bits;
}

bool BitArray::FindFirstSetBit(size_t& firstSetBitIndex) const
{
    size_t index = 0;
    size_t bitsPerElement = sizeof(BitData) * 8;
    size_t numElements = numBits / bitsPerElement;

    //Get the block of bits that contains the first set bit
    while ((bits[index] == BitData(0)) && (index < numElements))
    {
        index++;
    }

    if (index >= numElements) 
    {
        return false;
    }

    BitData currentBits = bits[index];
    unsigned long bitIndex;

#ifdef WIN32
    _BitScanForward(&bitIndex, currentBits);
#else
    _BitScanForward64(&bitIndex, currentBits);
#endif

    firstSetBitIndex = static_cast<size_t>(bitIndex);

    return true;
}

bool BitArray::FindFirstClearBit(size_t& firstClearBitIndex) const
{
    size_t index = 0;
    size_t bitsPerElement = sizeof(BitData) * 8;
    size_t numElements = numBits / bitsPerElement;

    //Get the block of bits that contains the first clear bit
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

    //The only difference between the first set bit and first clear bit is that we can do a bitwise NOT and then git the first set bit.
#ifdef WIN32
    _BitScanForward(&bitIndex, ~currentBits);
#else
    _BitScanForward64(&bitIndex, ~currentBits);
#endif

    firstClearBitIndex = static_cast<size_t>(bitIndex);

    return true;
}

void BitArray::ClearAllBits()
{
    size_t bitsPerElement = sizeof(BitData) * 8;
    size_t numElements = numBits / bitsPerElement;
    size_t numBytes = numElements * (bitsPerElement / 8);
    memset(bits, 0, numBytes);
}

void BitArray::SetAllBits()
{
    size_t bitsPerElement = sizeof(BitData) * 8;
    size_t numElements = numBits / bitsPerElement;
    size_t numBytes = numElements * (bitsPerElement / 8);
    //To set everything to 1 we need to make sure all bits in the char are set to 1
    memset(bits, 0xFF, numBytes);
}

bool BitArray::AreAllBitsClear() const
{
    size_t index = 0;
    size_t bitsPerElement = sizeof(BitData) * 8;
    size_t numElements = numBits / bitsPerElement;

    //Iterate through until you find any nonzero bit
    while ((bits[index] == 0) && (index < numElements))
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
    size_t bitsPerElement = sizeof(BitData) * 8;
    size_t numElements = numBits / bitsPerElement;

    while ((bits[index] == MaxValue) && (index < numElements))
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
    size_t bitsPerElement = sizeof(BitData) * 8;
    size_t elementIndex = bitNumber / bitsPerElement;
    size_t bitIndex = bitNumber % bitsPerElement;

    BitData element = bits[elementIndex];

    //Take the element, and OR it with a mask that has a 1 set on our desired bit
    bits[elementIndex] = element | (static_cast<BitData>(1) << bitIndex);
}

void BitArray::ClearBit(size_t bitNumber)
{
    size_t bitsPerElement = sizeof(BitData) * 8;
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
