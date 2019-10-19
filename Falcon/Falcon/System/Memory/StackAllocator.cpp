#include "StackAllocator.h"

namespace fmemory
{
	StackAllocator::StackAllocator()
		:m_stack_ref(nullptr),m_stack_top(nullptr),m_stack_end(nullptr)
	{
		//Allocate memory
		m_stack_ref = AllocateUnaligned(STACK_ALLOCATOR_MAX_SIZE);
		m_stack_top = static_cast<void**>(m_stack_ref);
		std::uintptr_t end_address = reinterpret_cast<std::uintptr_t>(m_stack_top)
									+ STACK_ALLOCATOR_MAX_SIZE;
		m_stack_end = reinterpret_cast<void**>(end_address);
		FL_ENGINE_INFO("{0},{1},{2}", reinterpret_cast<std::uintptr_t>(m_stack_ref), 
			reinterpret_cast<std::uintptr_t> (m_stack_top), 
			reinterpret_cast<std::uintptr_t> (m_stack_end));
	}

	StackAllocator::~StackAllocator()
	{
		Deallocate(m_stack_ref);
	}
	void* StackAllocator::GetMemoryBlock(std::size_t size_in_bytes)
	{
		if (m_stack_ref == nullptr)
		{
			FL_ENGINE_ERROR("ERROR: Stackallocator is not initialized");
			return nullptr;
		}

		if (IsSpaceLeft(size_in_bytes))
		{
			std::uintptr_t current_top = reinterpret_cast<std::uintptr_t>(m_stack_top);
			std::uintptr_t block_end = current_top + size_in_bytes;

			void* block_head = reinterpret_cast<void*>(current_top);
			m_stack_top = reinterpret_cast<void**>(block_end);

			FL_ENGINE_INFO("{0},{1}", reinterpret_cast<std::uintptr_t>(m_stack_top),
				reinterpret_cast<std::uintptr_t>(block_head));
			return block_head;
		}
		else
		{
			FL_ENGINE_ERROR("ERROR: Stackallocator is out of space");
			return nullptr;
		}
	}
	void* StackAllocator::FreeMemoryBlock(void* ref, std::size_t size_in_bytes)
	{
		return nullptr;
	}
	bool StackAllocator::IsSpaceLeft(std::size_t size)
	{
		std::uintptr_t current_top = reinterpret_cast<std::uintptr_t>(m_stack_top);
		std::uintptr_t end_address = reinterpret_cast<std::uintptr_t>(m_stack_end);

		FL_ENGINE_INFO("{0},{1},{2}", current_top,end_address, end_address - current_top);
		if (end_address - current_top > size)
			return true;
		return false;
	}
}