#ifndef STACK_ALLOCATOR_H
#define STACK_ALLOCATOR_H

#include <cstdlib>
#include <Log.h>
#include "Allocator.hpp"

#define STACK_ALLOCATOR_MAX_SIZE 3221225472 //1GB

namespace fmemory {
	class StackAllocator
	{
	private:
		void**	m_stack_top;
		void*   m_stack_ref;
		void**  m_stack_end;

	public:

		StackAllocator();
		StackAllocator(StackAllocator& other) = delete;
		~StackAllocator();


		void* GetMemoryBlock(std::size_t size_in_bytes);

		void* FreeMemoryBlock(void* ref,std::size_t size_in_bytes);

		inline void Flush() { m_stack_top = reinterpret_cast<void**>(m_stack_ref); }
		bool IsSpaceLeft(std::size_t size);
	};
}


#endif
