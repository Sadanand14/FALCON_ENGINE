#ifndef ALLOCATOR_HPP
#define ALLOCATOR_HPP

#include <cstddef>
#include "Log.h"

namespace fmemory {
	void* AllocateUnaligned(std::size_t sz_in_byte);
	void  Deallocate(void * memref);

	void* AllocateAligned(std::size_t sz_in_byte, std::size_t alignment);
	void  DeallocateAligned(void * memref);
}

#endif
