#ifndef PHYSICSSYSTEM_H
#define PHYSICSSYSTEM_H

#include "boost/container/vector.hpp"
#include "Core/EntityInterface.h"
#include "System/Memory/fmemory.h"
#include "System/Physics/Physics.h"
#include "System/Physics/PXMathUtils.h"

class PhysicsSystem
{
public:
	static void update(float dt, boost::container::vector<Entity*, fmemory::STLAllocator<Entity*>>* entity, const size_t& entity_count);
};

#endif