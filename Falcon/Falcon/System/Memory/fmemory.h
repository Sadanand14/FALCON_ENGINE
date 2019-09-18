#ifndef FMEMORY_H
#define FMEMORY_H

#include "PoolAllocator.hpp"
#include "STLMemoryInterface.h"
#include "MemoryInterface.h"
#include "Log.h"

#define INIT_MEMORY_MANAGER      fmemory::MemoryManagerInit()
#define MEMORY_MANAGER_SHUT_DOWN fmemory::MeoryManagerShutDown()


//template<class T,typename t1,typename t2,typename t3>
//T* fnew(t1 t_1, t2 t_2, t3 t_3)
//{
//	return new(fmemory::Allocate(sizeof(T))) T(t_1, t_2, t_3);
//}



template<typename T, typename... Args>
T* fnew(Args... args)
{
	return new (fmemory::Allocate(sizeof(T))) T(args...);
}


template <typename T>
void fdelete(T* ptr)
{
	reinterpret_cast<T*>(ptr)->~T();
	fmemory::Free(ptr,sizeof(T));
}



#endif //FMEMORY_H