#ifndef STACK_ALLOCATOR_H
#define STACK_ALLOCATOR_H

#include <cstdlib>
#include "System/Log.h"
#include "Allocator.hpp"

#define STACK_ALLOCATOR_MAX_SIZE 1073741824 //1GB

namespace fmemory {
	class StackAllocator
	{
	private:
		void**	m_stack_top;
		void* m_stack_ref;
		void** m_stack_end;

	public:

		StackAllocator();
		StackAllocator(StackAllocator& other) = delete;
		~StackAllocator();
		void* GetMemoryBlock(std::size_t size_in_bytes);

		void* FreeMemoryBlock(void* ref);

		bool IsSpaceLeft(std::size_t size);
	};
}


#endif
