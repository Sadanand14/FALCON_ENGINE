#ifndef ALLOCATOR_HPP
#define ALLOCATOR_HPP

#include <Types.h>
#include <cstddef>
#include "Log.h"


namespace fmemory {

	/**
	* Allocates unaligned memory.
	* @param[in] size_t size in bytes 
	*/
	void* AllocateUnaligned(std::size_t sz_in_byte);

	/**
	* Deallocates unaligned memory.
	* @param[in] void* pointer to the memory
	*/
	void  Deallocate(void * memref);


	/**
	* Allocates aligned memory.
	* @param[in] size_t size in bytes
	* @param[in] size_t alignment
	*/
	void* AllocateAligned(std::size_t sz_in_byte, std::size_t alignment);

	/**
	* Deallocates aligned memory.
	* @param[in] void* pointer to the memory
	*/
	void  DeallocateAligned(void * memref);
}

#endif
