#ifndef MEMORYINTERFACE_H
#define MEMORYINTERFACE_H

#include <cstddef>
namespace fmemory
{
	extern bool  MemoryManagerInit();
	extern void* Allocate(const std::size_t size);
	extern void  Free(void * ref, const std::size_t size);
	extern bool  MeoryManagerShutDown();
}
#endif // !MEMORYINTERFACE_H
