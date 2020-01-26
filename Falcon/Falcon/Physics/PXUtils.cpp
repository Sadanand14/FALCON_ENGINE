#include "PXUtils.h"
#include "Log.h"
namespace physics 
{

	void* FLPxAllocator::allocate(size_t size, const char* typeName, const char* filename, int line)
	{
		try
		{
			return fmemory::Allocate(size);
		}
		catch (std::exception& e)
		{
			FL_ENGINE_ERROR("ERROR: Failed to allocate the memory at {0}, in {1}.", line, filename);
			FL_ENGINE_ERROR("ERROR: {0}", e.what());
			return nullptr;
		}
	}

	void FLPxAllocator::deallocate(void* ptr)
	{
		try
		{

		}
		catch (std::exception & e)
		{
			FL_ENGINE_ERROR("ERROR:Failed to deallocate the memory for physx component with {0}", e.what());
		}
	}

	void FLPxErrorCallback::reportError(physx::PxErrorCode::Enum code, const char* message, const char* file, int line)
	{
		FL_ENGINE_ERROR("ERROR: PhysX error. {0} , {1}, in {2}, at {3}", code, message, file, line);
	}

}
