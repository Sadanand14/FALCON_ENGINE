#ifndef STACKSTLMEMORYINTERFACE_H
#define STACKSTLMEMORYINTERFACE_H

#include <memory>
#include "Log.h"
#include "MemoryInterface.h"


/**
* STL container constructors take in an allocators which they use to allocate memory for intenal object allocations.
* By defualt it uses, std::allocators. Following class provides similar interface which enables STL containers to use
* our custome allocators.Please use following link to understand std::allocator,
* https://en.cppreference.com/w/cpp/memory/allocator
*
* All standard containers access these via std::allocators. Thus we don't need to override the construct and destroy methods.
* Construct creats object inplace with placement new and destroy method calls destructors for the same object without freeing the pointer.
* Please refer following link for details,
* https://en.cppreference.com/w/cpp/memory/allocator_traits
*/

namespace fmemory {


	template <class T>
	class StackSTLAllocator
	{

	public:
		//Keeping the naming convention same as std::allocator class.
		typedef T value_type;
		typedef T* pointer;
		typedef T& reference;



		template <class U>
		struct rebind
		{
			typedef StackSTLAllocator<U> other;
		};


		/**
		* Allocators are stateless so we don't need to define the anything in constructors and destructors.
		*/
		StackSTLAllocator() = default;
		~StackSTLAllocator() = default;


		template <class U>
		StackSTLAllocator(const StackSTLAllocator<U>& ref)
		{
		}

		/**
		* Allocator for the stl containers.
		* Takes in variable count and returns pointer to specified type.
		*/
		pointer allocate(size_t count)
		{

			FL_ENGINE_INFO("Allocating using StackSTLAllocator Requesting {0} , {1}", sizeof(T), sizeof(T) * count);

			return reinterpret_cast<T*>(AllocateOnStack(sizeof(T) * count));
		}

		/**
		* Deallocator for the stl containers.
		* Takes in variable count and pointer to specified type.
		* Then calls custom deallocator.
		*/
		void deallocate(pointer p, size_t count)
		{
			//FL_ENGINE_INFO("Dellocating using StackSTLAllocator.");
			Free(p, sizeof(T) * count);
		}


		/**
		* obtains the address of an object, even if operator& is overloaded
		*/
		pointer address(reference x) const
		{
			return &x;
		}

	};


	/**
		* Compares two default allocators. Since default allocators are stateless, two default allocators are always equal.
		* https://en.cppreference.com/w/cpp/memory/allocator/operator_cmp
		* Always returns true.
	*/
	template <class T, class U>
	constexpr bool operator== (const StackSTLAllocator<T>&, const StackSTLAllocator<U>&) noexcept
	{
		return true;
	}


	/**
		* Compares two default allocators. Since default allocators are stateless, two default allocators are always equal.
		* https://en.cppreference.com/w/cpp/memory/allocator/operator_cmp
		* Always returns false.
	*/
	template <class T, class U>
	constexpr bool operator!= (const StackSTLAllocator<T>&, const StackSTLAllocator<U>&) noexcept
	{
		return false;
	}


}
#endif //STACKSTLMEMORYINTERFACE_H