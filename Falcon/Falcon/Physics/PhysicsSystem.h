#ifndef PHYSICSSYSTEM_H
#define PHYSICSSYSTEM_H

#include "boost/container/vector.hpp"
#include "Core/EntityInterface.h"
#include "System/Memory/fmemory.h"
#include "Physics.h"
#include "PXMathUtils.h"

/**
* @class Handles updates on the entities based on the physics.
*/
class PhysicsSystem
{
public:
	/**
	* Updates the entities after physics simulations.
	* @param delta time,
	* @param vector of entities with physics compoenent
	* @param count of entities
	*/
	static void update(float dt, boost::container::vector<Entity*, fmemory::STLAllocator<Entity*>>* entity, const size_t& entity_count);
};

#endif