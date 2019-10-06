#include "Allocator.hpp"

#include <cstdlib>
#include <cassert>
#include <exception>
#include <iostream>
#include "Log.h"
namespace fmemory {


	/*
		Allocates the memory block of requested size.Returns nullptr on failure.
	*/
	void * AllocateUnaligned(std::size_t sz_in_byte) {
		try
		{
			void * main_memory_block = malloc(sz_in_byte);
			return main_memory_block;
		}
		catch (std::exception& e)
		{
			FL_ENGINE_ERROR("ERROR:{0}",e.what());
			return nullptr;
		}
		
	}

	/*
		Deallocates the memory assocated with the pointer
	*/
	void Deallocate(void * memref)
	{
		try 
		{
			//FL_ENGINE_INFO("Deallocating {0}", memref);
			free(memref);
		}
		catch (std::exception& e)
		{

			FL_ENGINE_ERROR("ERROR:{0}", e.what());
			std::cin.get();
			std::cin.get();
		}
	}

	void * AllocateAligned(std::size_t sz_in_byte, std::size_t alignment)
	{
		assert(alignment >= 1); //Minimum alignment value should be 1
		assert(alignment <= 128);//Max alignment value should be 128
		assert((alignment&alignment - 1) == 0); //Checking if alignment is in 2's power

		//Total size required with alignment
		std::size_t total_size = sz_in_byte + alignment;

		//Allocating unaligned memory
		std::uintptr_t raw_memory_address = reinterpret_cast<std::uintptr_t>(AllocateUnaligned(total_size));
		//std::cout << reinterpret_cast<void*>(raw_memory_address) << std::endl;;

		//Calculating the adjustment for address by masking of lower bits
		//Using gregory aproach
		std::size_t  mask = alignment - 1;
		std::uintptr_t misalignment = raw_memory_address & mask;
		std::ptrdiff_t adjustment = alignment - misalignment;

		//Fixing memory based on adjustment
		std::uintptr_t aligned_memory_address = raw_memory_address + adjustment;
		
		//FL_ENGINE_INFO("Allignement for memory={0} mask= {1} Adjustment={2}", alignment,mask,adjustment);
		//FL_ENGINE_INFO("Alignement Memory Address = {0} ", aligned_memory_address);

		//storing adjustment value
		std::uint8_t* paligned_memory = reinterpret_cast<std::uint8_t*>(aligned_memory_address);
		paligned_memory[-1] = static_cast<uint8_t>(adjustment);
		return static_cast<void*>(paligned_memory);
	}

	void DeallocateAligned(void * memref)
	{
		
		try
		{
			
			const std::uint8_t* paligned_memory = reinterpret_cast<const std::uint8_t*>(memref);
			if (paligned_memory == nullptr)
				return;
			std::uintptr_t aligned_address = reinterpret_cast<std::uintptr_t>(memref);

			std::ptrdiff_t adjustment = static_cast<std::ptrdiff_t>(paligned_memory[-1]);

			std::uintptr_t unaligned_address = aligned_address - adjustment;
			void* raw_memory_address = reinterpret_cast<void*>(unaligned_address);
			Deallocate(raw_memory_address);
		}
		catch (std::exception& e)
		{
			FL_ENGINE_ERROR("ERROR: Failed to deallocate. {0}", e.what());
		}
	}


}