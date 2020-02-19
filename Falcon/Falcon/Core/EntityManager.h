#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H

#include <string>
#include <fstream>
#include <boost/container/vector.hpp>
#include <boost/unordered_map.hpp>
#include <glm/glm.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/vec3.hpp>
#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

#include <AssetManager.h>
#include <Memory/fmemory.h>
#include "EntityInterface.h"
#include <Log.h>
#include "Types.h"

class EntityManager
{
private:
	//boost::container::vector<Entity, fmemory::STLAllocator<Entity>> m_entities;

	//static void LoadMesh(const std::string& meshPath);
	//static void LoadMaterial(const std::string& matPath);
public:
	//void LoadEntity(const char* EntityPathFile);
	static Entity* CreateEntity(const char*  objTemplate, glm::vec3 pos, glm::quat rot, glm::vec3 scale);
	
	static void SaveScene(const char* sceneFilePath);
	static void CloseScene();
};

#endif //ENTITY_MANAGER_H
