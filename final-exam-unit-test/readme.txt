readme.txt for FinalExam

In order to more easily manage the construction and destruction of the FixedSizeAllocators,
I allocated the BitArrays with my defaultHeapManager and passed them into the FSAs. This is done in 
MemorySystem.cpp line 18-31. 

I also added a get function for the base address of the FSAs to more easily
free those memory blocks at the end. (MemorySystem.cpp line 44-50)

In malloc() and free() I made sure to have null checks for my allocators so that if someone
tries to call malloc() or free() before the allocators have been initialized they can still
get/free memory. (Allocators.cpp lines 17, 39, 51, 69)

My contains() function in the FSAs uses pointer arithmetic to determine if the address given
is within the memory allocated for the given FSA. (FixedSizeAllocator.cpp line 62-74)

When freeing in the FSAs I have warning checks for pointers that don't belong to the FSA,
pointers that are already marked as free, and also a check for a pointer that isn't aligned
to the FSA's block size (FixedSizeAllocator.cpp line 43, 53, 88).