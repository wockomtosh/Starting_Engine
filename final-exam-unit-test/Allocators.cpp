#include <inttypes.h>
#include <malloc.h>
#include "MemorySystem.h"

#include <stdio.h>


void * __cdecl malloc(size_t i_size)
{
	//printf("malloc %zu\n", i_size);

	void* returnValue;

	//We need to malloc certain things before the FSAs are ready (mostly the bitarrays that we pass into them)
	if (FSAList)
	{
		//Check all the FSAs to see if there's a match
		for (int i = 0; i < numFSAs; i++)
		{
			if (i_size <= FSASizes[i].blockSize)
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

	return defaultHeapManager->alloc(i_size);
}

void __cdecl free(void * i_ptr)
{
	//printf("free 0x%" PRIXPTR "\n", reinterpret_cast<uintptr_t>(i_ptr));

	//Find which allocator it belongs to and call free there
	for (int i = 0; i < numFSAs; i++)
	{
		if (FSAList[i]->contains(i_ptr))
		{
			FSAList[i]->freeBlock(i_ptr);
		}
	}

#ifdef _DEBUG
	if (!defaultHeapManager->contains(i_ptr))
	{
		std::cout << "ERROR: no allocators contain pointer " << i_ptr << std::endl;
	}
#endif

	//If it doesn't belong anywhere else then we send it to the default
	defaultHeapManager->freeBlock(i_ptr);
}

void * operator new(size_t i_size)
{
	//printf("new %zu\n", i_size);

	//We need to malloc and then call the constructor?
	return malloc(i_size);
}

void operator delete(void * i_ptr)
{
	//printf("delete 0x%" PRIXPTR "\n", reinterpret_cast<uintptr_t>(i_ptr));

	return free(i_ptr);
}

void * operator new[](size_t i_size)
{
	//printf("new [] %zu\n", i_size);

	return malloc(i_size);
}

void operator delete [](void * i_ptr)
{
	//printf("delete [] 0x%" PRIXPTR "\n", reinterpret_cast<uintptr_t>(i_ptr));

	return free(i_ptr);
}