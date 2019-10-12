#include "StackAllocator.h"

namespace fmemory
{
	StackAllocator::StackAllocator()
	{
		//Allocate memory
		m_stack_ref = AllocateUnaligned(STACK_ALLOCATOR_MAX_SIZE);
		m_stack_top = static_cast<void**>(m_stack_ref);
		std::uintptr_t end_address = reinterpret_cast<std::uintptr_t>(m_stack_top)
									+ STACK_ALLOCATOR_MAX_SIZE;
		m_end_address = reinterpret_cast<void**>(end_address);
		FL_ENGINE_INFO("{0},{1},{2}", reinterpret_cast<std::uintptr_t>(m_stack_ref), 
			reinterpret_cast<std::uintptr_t> (m_stack_top), 
			reinterpret_cast<std::uintptr_t> (m_end_address));
	}

	StackAllocator::~StackAllocator()
	{
		Deallocate(m_stack_ref);
	}
	void* StackAllocator::GetMemoryBlock(std::size_t size_in_bytes)
	{
		return nullptr;
	}
	void* StackAllocator::FreeMemoryBlock(void* ref)
	{
		return nullptr;
	}
	bool StackAllocator::IsSpaceLeft(std::size_t size)
	{
		std::uintptr_t current_top = reinterpret_cast<std::uintptr_t>(m_stack_top);
		std::uintptr_t end_address = reinterpret_cast<std::uintptr_t>(m_stack_top);
		if (end_address - current_top > size)
			return true;
		return false;
	}
}