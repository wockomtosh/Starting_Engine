
#include "HeapManagerProxy.h"
// You'll need to include the header file for your Heap allcoator
// #include "HeapManager.h"

// You may need to include other files depending on your needs
#include <assert.h>
#include <stdio.h>

// This is your include file for your heap manager. It should have a class definition for HeapManager.
// If you use another name for the class, header or functions just replace what needs to be in here.
namespace HeapManagerProxy
{

// You'll need to provide implementations of all these functions
HeapManager * CreateHeapManager( void * i_pMemory, size_t i_sizeMemory, unsigned int i_numDescriptors )
{
	// Creates a new Heap allocator and return a pointer to it
}

void Destroy( HeapManager * i_pManager )
{
	assert( i_pManager );
	// Shuts down this Heap allocator instance
}

void * alloc( HeapManager * i_pManager, size_t i_size )
{
	assert( i_pManager );
	// allocate from this Heap allocator
}

void * alloc( HeapManager * i_pManager, size_t i_size, unsigned int i_alignment )
{
	assert( i_pManager );
	// allocate from this Heap allocator with the given alignment
}

bool free( HeapManager * i_pManager, void * i_ptr )
{
	assert( i_pManager );
	// free the memory at this pointer
}

void Collect( HeapManager * i_pManager )
{
	assert( i_pManager );
	// This should do block coallescing
}


bool Contains( const HeapManager * i_pManager, void * i_ptr )
{
	assert( i_pManager );
	// returns true if the HeapManager manages memory that includes this pointer
}

bool IsAllocated( const HeapManager * i_pManager, void * i_ptr )
{
	assert( i_pManager );
	// returns true if the HeapManager is tracking an allocation that starts at this pointer
}

size_t GetLargestFreeBlock( const HeapManager * i_pManager )
{
	assert( i_pManager );
	// optional
}

size_t GetTotalFreeMemory( const HeapManager * i_pManager )
{
	assert( i_pManager );
	// optional
}

void ShowFreeBlocks( const HeapManager * i_pManager )
{
	assert( i_pManager );
	// optional
}

void ShowOutstandingAllocations( const HeapManager * i_pManager )
{
	assert( i_pManager );
	// optional

}

} // namespace HeapManagerProxy