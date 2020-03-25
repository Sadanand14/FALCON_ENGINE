#ifndef MEMORYINTERFACE_H
#define MEMORYINTERFACE_H

#include <Types.h>
#include <cstddef>
namespace fmemory
{
	static boost::mutex MemMtx;

	/**
	* <p>Initiates the memory manager. Needs to be called 
	* before running any code which uses any fmemory features.</p>
	* @return true if success, otherwise false
	*/
	bool  MemoryManagerInit();

	/**
	* <p>Gets you a memory block from available pool.
	* If no pool can accomadate required size it'll create a 
	* block using standard memory functions.<p>
	*
	* @param size size of required memory block
	* @return pointer to the allocated space
	*/
	void* Allocate(const std::size_t size);

	/**
	* <p>Frees the memory block</p>
	* @param ref pointer to the memory block
	* @param size size of object being deleted, helps find the pool
	*/
	void  Free(void * ref, const std::size_t size);

	/**
	* Shuts down memory manager. Needs to be called at the end of 
	* engine's life cycle so as to make sure no memory leaks are 
	* present.
	* @return true if success, otherwise false
	*/
	bool  MeoryManagerShutDown();


	void*  AllocateOnStack(const std::size_t size);

	bool FlushStack();
}
#endif // !MEMORYINTERFACE_H
