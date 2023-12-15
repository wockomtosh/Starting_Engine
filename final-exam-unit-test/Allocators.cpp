#include <inttypes.h>
#include <malloc.h>
#include "MemorySystem.h"

#include <stdio.h>


void * __cdecl malloc(size_t i_size)
{
#ifdef _DEBUG
	printf("malloc %zu\n", i_size);
#endif

	void* returnValue;

	//We need to malloc certain things before the FSAs are ready (mostly the bitarrays that we pass into them)
	if (FSAList)
	{
		//Check all the FSAs to see if there's a match
		for (int i = 0; i < numFSAs; i++)
		{
			if (i_size <= FSASizes[i].blockSize)
			{
				if (FSAList[i])
				{
					returnValue = FSAList[i]->alloc();
					if (returnValue != nullptr)
					{
						return returnValue;
					}
					//If the FSA returns null we could try to use a bigger FSA, but in this case it'll probably be easier to just use the heap allocator.
					else
						break;
				}
			}
		}
	}

	if (defaultHeapManager)
	{
		return defaultHeapManager->alloc(i_size);
	}
	
	//If the default isn't there we need to use something
	return _aligned_malloc(i_size, 4);
}

void __cdecl free(void * i_ptr)
{
	//Find which allocator it belongs to and call free there
	if (FSAList)
	{
		for (int i = 0; i < numFSAs; i++)
		{
			if (FSAList[i])
			{
				if (FSAList[i]->contains(i_ptr))
				{
#ifdef _DEBUG
					printf("free %zu\n", FSASizes[i].blockSize);
#endif
					return FSAList[i]->freeBlock(i_ptr);
				}
			}
		}
	}

	//If it doesn't belong anywhere else then we send it to the default
	if (defaultHeapManager)
	{
#ifdef _DEBUG
		if (!defaultHeapManager->contains(i_ptr))
		{
			printf("ERROR: no allocators contain pointer 0x%" PRIXPTR "\n", reinterpret_cast<uintptr_t>(i_ptr));
		}
#endif
		return defaultHeapManager->freeBlock(i_ptr);
	}

	//If the default isn't there we need to use something
	return _aligned_free(i_ptr);
}

void * operator new(size_t i_size)
{
	return malloc(i_size);
}

void operator delete(void * i_ptr)
{
	return free(i_ptr);
}

void * operator new[](size_t i_size)
{
	return malloc(i_size);
}

void operator delete [](void * i_ptr)
{
	return free(i_ptr);
}