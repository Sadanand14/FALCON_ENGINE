#ifndef FNEW_H
#define FNEW_H
#include "Log.h"
namespace fmemory
{
	template<typename T, typename... Args>
	T* fnew(Args... args)
	{
		return new (fmemory::Allocate(sizeof(T))) T(args...);
	}

	template <typename T, std::size_t count, typename... Args >
	T * fnew(Args... args)
	{
		//Allocating the memory
		T* ptr =static_cast<T*>(fmemory::Allocate(sizeof(T) * count));
		FL_ENGINE_ERROR("Allocating Array: Main Block starts at {0}", static_cast<void*>(ptr));
		//Constructing the object
		for (size_t itr = 0; itr < count; ++itr)
		{
			FL_ENGINE_INFO("Constructing at {0}", static_cast<void*>(ptr + itr));
			new(ptr + itr) T(args...);
		}

		return ptr;
	}

	template <typename T>
	void fdelete(T* ptr)
	{
		reinterpret_cast<T*>(ptr)->~T();
		fmemory::Free(ptr, sizeof(T));
	}

	template<typename T, std::size_t count>
	void fdelete(T * ptr)
	{
		FL_ENGINE_ERROR("Deleting Array: Main Block starts at {0}", static_cast<void*> (ptr));
		reinterpret_cast<T*>(ptr)->~T();
		fmemory::Free(ptr,sizeof(T)*count);
	}


}

#endif