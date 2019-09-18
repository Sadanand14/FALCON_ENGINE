#ifndef FMEMORY_H
#define FMEMORY_H

#include "PoolAllocator.hpp"
#include "STLMemoryInterface.h"
#include "MemoryInterface.h"


#define INIT_MEMORY_MANAGER      fmemory::MemoryManagerInit()
#define MEMORY_MANAGER_SHUT_DOWN fmemory::MeoryManagerShutDown()


template<class T, class... Args>
T* fnew(...)
{
	return new (fmemory::Allocate(sizeof(T))) T(Args...);
}

template <class T>
void fdelete(T* ptr)
{
	reinterpret_cast<T*>ptr->~T();
	fmemory::Free(ptr,sizeof(T);
}



#endif //FMEMORY_H