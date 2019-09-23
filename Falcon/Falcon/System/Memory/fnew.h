#ifndef FNEW_H
#define FNEW_H
#include "Log.h"
namespace fmemory
{
	template<typename T, typename... Args>
	T* fnew(Args... args)
	{
		try
		{
			return new (fmemory::Allocate(sizeof(T))) T(args...);
		}
		catch (std::exception e)
		{
			FL_ENGINE_ERROR("ERROR: Failed to allocate memory.{0}", e.what());
			return nullptr;
		}
	}

	template <typename T, std::size_t& count, typename... Args >
	T * fnew(Args... args)
	{
		if (count <= 0)
		{
			FL_ENGINE_ERROR("ERROR:Invalid value of count for allocation provided. Not proceeding with the allocation. {0}", count);
			return nullptr;
		}

		try {
			//Allocating the memory
			T* ptr = static_cast<T*>(fmemory::Allocate(sizeof(T) * count));
			FL_ENGINE_ERROR("Allocating Array: Main Block starts at {0}", static_cast<void*>(ptr));
			//Constructing the object
			for (size_t itr = 0; itr < count; ++itr)
			{
				FL_ENGINE_INFO("Constructing at {0}", static_cast<void*>(ptr + itr));
				new(ptr + itr) T(args...);
			}
			return ptr;
		}
		catch (std::exception e)
		{
			FL_ENGINE_ERROR("ERROR: Failed to allocate memory.{0}",e.what());
			return nullptr;
		}
	}



	template <typename T>
	void fdelete(T* ptr)
	{
		try {
			reinterpret_cast<T*>(ptr)->~T();
			fmemory::Free(ptr, sizeof(T));
		}
		catch (std::exception e)
		{
			FL_ENGINE_ERROR("ERROR: Failed to release memory.{0}", e.what());
		}
	}

	template <typename T, std::size_t& count>
	void fdelete(T * ptr)
	{
		if (count <= 0)
		{
			FL_ENGINE_ERROR("ERROR:Invalid value of count for deallocation provided. Not proceeding with the deallocation. {0}",count);
			return;
		}
		
		try {
			//destructing the objects.
			for(size_t itr = 0;itr<count;++itr)
				reinterpret_cast<T*>(ptr+itr)->~T();

			//Freeing complete memory block.
			fmemory::Free(ptr, sizeof(T) * count);
			return;
		}
		catch (std::exception e)
		{
			FL_ENGINE_ERROR("ERROR: Failed to release memory.{0}", e.what());
			return;
		}
	}


}

#endif