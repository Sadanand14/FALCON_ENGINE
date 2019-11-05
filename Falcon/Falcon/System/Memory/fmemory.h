#ifndef FMEMORY_H
#define FMEMORY_H


#include "PoolAllocator.hpp"
#include "StackAllocator.h"
#include "STLMemoryInterface.h"
#include "StackSTLAllocator.h"
#include "MemoryInterface.h"
#include "fnew.h"

#define INIT_MEMORY_MANAGER      fmemory::MemoryManagerInit()
#define MEMORY_MANAGER_SHUT_DOWN fmemory::MeoryManagerShutDown()

#endif