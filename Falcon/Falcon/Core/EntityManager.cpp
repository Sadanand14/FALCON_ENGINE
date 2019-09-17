#include "EntityManager.h"

Entity* EntityManager::CreateEntity() 
{
	return new Entity();
}