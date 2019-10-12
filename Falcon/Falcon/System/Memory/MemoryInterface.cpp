#include <vector>

#include "Allocator.hpp"
#include "MemoryInterface.h"
#include "PoolAllocator.hpp"


namespace fmemory {
	//memory block size list
	static const size_t BLOCK_SIZES[] = {
		//4 bytes increaments
		8,12,16,20,24,28,32,36,40,44,48,52,56,60,
		64,68,72,76,80,84,88,92,96,

		//32 bytes increaments
		128,160,192,224,256,288,320,352,384,416,448,512,544,576,
		608,640,

		////64 bytes increaments
		704, 768, 832, 896, 960, 1024

	};

	static const unsigned POOL_SIZE = 8192; // Each pool will alocate 4096 bytes


	static const std::size_t BLOCK_SIZE_LIST_COUNT = sizeof(BLOCK_SIZES) / sizeof(BLOCK_SIZES[0]);
	static const std::size_t MAX_BLOCK_SIZE = BLOCK_SIZES[BLOCK_SIZE_LIST_COUNT - 1];

	static unsigned* pool_lookup = nullptr;
	static PoolAllocator* pool_allocators = static_cast<PoolAllocator*>(fmemory::AllocateUnaligned(sizeof(PoolAllocator) * BLOCK_SIZE_LIST_COUNT));
	
	
	//This is the method which initializes the memory manager. 
	//Needs to be called when all the subsystems are initiated. And should be the first one.
	//For now alignement is default to 4. In future need to add the feature to pass in user specified value.
	bool MemoryManagerInit()
	{
		bool IsInitialized = false;
		//intializing the lookup pointer
		try
		{
			PoolAllocator* tmp;
			pool_lookup = new unsigned[MAX_BLOCK_SIZE + 1];
			unsigned block_index = 0;
			for (unsigned index = 0; index <= MAX_BLOCK_SIZE; ++index)
			{
				if (index > BLOCK_SIZES[block_index])
					++block_index;
				pool_lookup[index] = block_index;
			}

			for (unsigned index = 0; index < BLOCK_SIZE_LIST_COUNT; ++index)
			{
				
				tmp = pool_allocators + index;
				tmp = new (tmp) PoolAllocator;
				//FL_ENGINE_INFO("INFO: Creating pool for {0},  {1}, {2}", index, BLOCK_SIZES[index],static_cast<void*>(tmp));
				tmp->CreatePool(BLOCK_SIZES[index], POOL_SIZE / BLOCK_SIZES[index],
					(BLOCK_SIZES[index] < 16) ? 4 : 16);
			}
			IsInitialized = true;
			FL_ENGINE_INFO("INFO: Memory manager initialized.");
		}
		catch (std::exception& e)
		{
			FL_ENGINE_ERROR("ERROR: Memory initialization failed. {0}", e.what());
			return IsInitialized;
		}

		return IsInitialized;
	}



	//This method will clear all the memory pools and shut down the manager. 
	//Needs to be called at very end of engine shut down.
	bool MeoryManagerShutDown()
	{
		try
		{

			//delete pool_lookup;
			PoolAllocator* tmp;
			for (int i = BLOCK_SIZE_LIST_COUNT - 1; i >= 0; --i)
			{
				tmp = pool_allocators + i;
				//FL_ENGINE_INFO("{0}, {1}", (void*)tmp, tmp->GetPoolMemLocation());
				tmp->~PoolAllocator();
			}
			fmemory::Deallocate(pool_allocators);
			delete pool_lookup;
			//allocators are on stack so no need to delete them here
			FL_ENGINE_INFO("INFO: Memory Manger shut down properly.");
			return true;
		}
		catch (std::exception& e)
		{
			FL_ENGINE_ERROR("ERROR: Failed to shut down the memory manager.{0}", e.what());
			return false;
		}
	}




	/**
	*
	* Looks up block for required size into pool memory. 
	* @return PoolAllocator* on success otherwise null
	*/
	static PoolAllocator* LookUp(const size_t size)
	{

		//Return the block if available
		if (size > MAX_BLOCK_SIZE)
		{
			FL_ENGINE_WARN("WARNING: Unable to find the block.");
			return nullptr;
		}
		else
		{
			//return the lookup size
			return pool_allocators + pool_lookup[size];
		}
	}


	void* Allocate(const std::size_t size)
	{
		PoolAllocator* pool = LookUp(size);

		if (pool)
		{
			return pool->GetBlocKFromPool();
		}
		else
		{
			//If size is greater than the max block size, it is allocated using std::malloc and needs to be freed the same way.
			//Planning to use our allocators instead once we override new and delete 
			FL_ENGINE_WARN("WARNING: Allocating memeory with the std functions.");
			return std::malloc(size);
		}
	}

	



	void Free(void* ref, const std::size_t size)
	{
		if (ref == nullptr)
			return;
		//If size is greater than the max block size, it is allocated using std::malloc and needs to be freed the same way.
		//Planning to use our allocators instead once we override new and delete operators
		if (size > MAX_BLOCK_SIZE)
		{
			std::free(ref);
		}
		else
		{
			PoolAllocator* pool = LookUp(size);
			pool->Release(ref);
		}
	}


}