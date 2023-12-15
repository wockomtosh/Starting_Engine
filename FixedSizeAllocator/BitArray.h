#pragma once
#include <cstdint>

class BitArray
{
//TODO: FIX THIS
#ifdef WIN32
	typedef uint32_t BitData;
#else
	typedef uint64_t BitData;
#endif

public:
	BitArray(size_t numBits, bool initToZero);
	~BitArray();
	//copy constructor and the other big 3/5 stuff?

	bool FindFirstSetBit(size_t& firstSetBitIndex) const;
	bool FindFirstClearBit(size_t& firstClearBitIndex) const;

	void ClearAllBits();
	void SetAllBits();

	bool AreAllBitsClear() const;
	bool AreAllBitsSet() const;

	inline bool IsBitSet(size_t bitNumber) const;
	inline bool IsBitClear(size_t bitNumber) const;

	void SetBit(size_t bitNumber);
	void ClearBit(size_t bitNumber);

	bool operator[](size_t bitIndex) const;

private:
	BitData* bits;
	size_t numBits;

	const size_t bitsPerElement = sizeof(BitData) * 8;
};

inline bool BitArray::IsBitSet(size_t bitNumber) const
{
	size_t elementIndex = bitNumber / bitsPerElement;
	size_t bitIndex = bitNumber % bitsPerElement;

	BitData element = bits[elementIndex];

	//Do an AND to find out if the bit is 1. Because we are using just 1 set bit in our mask, the value will be positive only if the desired bit is set to 1.
	return element & (static_cast<BitData>(1) << bitIndex);
}

inline bool BitArray::IsBitClear(size_t bitNumber) const
{
	return !IsBitSet(bitNumber);
}