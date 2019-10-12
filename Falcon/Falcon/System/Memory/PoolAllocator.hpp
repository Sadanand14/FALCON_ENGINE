#ifndef  POOL_ALLOCATOR_H
#define  POOL_ALLOCATOR_H

#include <cstdlib>

namespace fmemory {

	/**
	* @class
	* <p>Pool allocator class provides functionality create pools of same sized memory 
 	* blocks.</p>
	* 
	*/

	class PoolAllocator
	{
	private:
		void *m_poolref;//Needs to be stored to free the memory 
		void **m_free_list_head;//Points to head of free node
		std::size_t m_num_of_elements;
		std::size_t m_element_size_in_bytes;
		std::size_t m_alignment;

	public:
		PoolAllocator();
		PoolAllocator(const std::size_t size_in_bytes, const std::size_t num_of_elements, const std::size_t alignment);
		
		void CreatePool(const std::size_t size_in_bytes, const std::size_t num_of_elements, const std::size_t alignment);

		void* GetBlocKFromPool();

		void Release(void* mem_ref);

		void* GetPoolMemLocation();

		/*
			Making sure compiler doesn't create default definations for following functions.
		*/

		//Copy Constructor and copy assignment
		PoolAllocator(PoolAllocator& poolRef) = delete;
		PoolAllocator& operator = (const PoolAllocator& poolRef) = default;

		//Double pointers related methods
		PoolAllocator(PoolAllocator&& refToPool) = delete;
		PoolAllocator& operator = (const PoolAllocator&& refToPool) = delete;


		~PoolAllocator();
	};


}
#endif // ! POOL_ALLOCATOR_H

