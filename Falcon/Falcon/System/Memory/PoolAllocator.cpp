#include <cassert>
#include <iostream>
#include <atomic>
#include "Allocator.hpp"
#include "PoolAllocator.hpp"

namespace fmemory {
	
	/**
	* <p> Default constructor </p>
	*
	*/
	PoolAllocator::PoolAllocator()
		:m_poolref(nullptr),m_free_list_head(nullptr),m_num_of_elements(0),m_element_size_in_bytes(0),m_alignment(4)
	{
	}

	/**
	*
	* Parameterized constructor
	* @param size_t element size in bytes sizeof(T)
	* @param size_t number of elements
	* @param size_t alignment required
	*/
	PoolAllocator::PoolAllocator(const std::size_t element_size_in_bytes, const std::size_t num_of_elements, const std::size_t alignment)
	{
		m_poolref = nullptr;
		m_free_list_head = nullptr;
		CreatePool(element_size_in_bytes, num_of_elements,alignment);
	}


	/**
	* Creats pool of memory for requested number of elements.
	* @param size_t element size in bytes sizeof(T)
	* @param size_t number of elements
	* @param size_t alignment required 
	*/

	void PoolAllocator::CreatePool(const std::size_t element_size_in_bytes, const std::size_t num_of_elements, const std::size_t alignment)
	{
		//Check if pool is not initalized.Doen't allow the memory to be allocated again for same pool.
		assert(m_poolref == nullptr);

		////Check for element size.
		//assert(element_size_in_bytes >= sizeof(void *));

		//Initializing class members
		m_element_size_in_bytes = element_size_in_bytes;
		m_num_of_elements = num_of_elements;
		m_alignment = alignment;
		std::size_t pool_size = m_num_of_elements * element_size_in_bytes;

		//Allocating memory
		m_poolref = AllocateAligned(pool_size,alignment);

		if (m_poolref)
		{
			//Creating list of free members
			m_free_list_head = static_cast<void**>(m_poolref);
			std::uintptr_t end_address = reinterpret_cast<std::uintptr_t>(m_free_list_head) 
					+ m_element_size_in_bytes * m_num_of_elements;

			for (std::size_t element_count = 0; element_count < num_of_elements; element_count++)
			{
				std::uintptr_t current_address = reinterpret_cast<std::uintptr_t>(m_free_list_head)
					+ element_count * m_element_size_in_bytes;
				std::uintptr_t next_address = current_address + m_element_size_in_bytes;

				void ** current_memory = reinterpret_cast<void**>(current_address);
				if (next_address >= end_address) {
					*current_memory = nullptr;
				}
				else
				{
					*current_memory = reinterpret_cast<void*>(next_address);
				}
			}

		}
		else
		{
			FL_ENGINE_ERROR("ERROR: Failed to allocate pool memory");
		}

	}

	/**
	* Returns next free block from pool.
	* @return void* returns pointer if block available else nullptr
	*/
	void * PoolAllocator::GetBlocKFromPool()
	{
		if (m_poolref == nullptr)
		{
			FL_ENGINE_ERROR("ERROR: Memory not allocated for the pool.");
			return nullptr;
		}

		if (m_free_list_head != nullptr)
		{
			void* memory_block = reinterpret_cast<void*>(m_free_list_head);
			m_free_list_head = static_cast<void**>(*m_free_list_head);
			return memory_block;
		}
		else
		{
			FL_ENGINE_ERROR("ERROR: Out of memory blocks.");
			return nullptr;
		}
	}


	/**
	* Releases the referenced block and adds back to free list.
	* @param void* pointer to memory_block
	*/
	void PoolAllocator::Release(void * memory_block)
	{
		if (memory_block == nullptr)
		{
			return;
		}

		if (m_poolref == nullptr)
		{
			std::cerr << "ERROR: Trying to free invalid memory" << std::endl;
			return;
		}

		if (m_free_list_head == nullptr)
		{
			m_free_list_head = reinterpret_cast<void**>(memory_block);
			*m_free_list_head = nullptr;
		}
		else
		{
			void** current_head = m_free_list_head;
			m_free_list_head = reinterpret_cast<void**>(memory_block);
			*m_free_list_head = reinterpret_cast<void*>(current_head);
		}

	}


	/**
	* Returns base pointer to the pool.
	* @return void* return pointer to the pool
	*/
	void* PoolAllocator::GetPoolMemLocation()
	{
		return m_poolref;
	}


	/**
	* Destructor. Deallocates all the memory and frees everything.
	*/
	PoolAllocator::~PoolAllocator()
	{
		DeallocateAligned(m_poolref);
		m_poolref = m_free_list_head = nullptr;
	}

}
