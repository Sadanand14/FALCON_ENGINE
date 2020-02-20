#ifndef PXUTILS_H
#define PXUTILS_H

#include <PxPhysicsAPI.h>
#include <Physx/pxshared/include/foundation/PxAllocatorCallback.h>
#include "Memory/fmemory.h"

namespace physics
{
	class FLPxAllocator : public physx::PxAllocatorCallback
	{

	public:

		void* allocate(size_t size, const char* typeName, const char* filename, int line) override;
		void deallocate(void* ptr) override;
	};


	/**
	* Custome error handler for physx logging. 
	*/
	class FLPxErrorCallback : public physx::PxErrorCallback
	{
	public:
		/**
		* Error handler method used by error callbacks in physx to log.
		*/
		void reportError(physx::PxErrorCode::Enum code, const char* message, const char* file, int line) override;
	};
}
#endif PXUTILS_H
