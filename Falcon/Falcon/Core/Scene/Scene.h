#ifndef SCENE_H
#define SCENE_H

#include <string>
#include <fstream>
#include <boost/container/vector.hpp>
#include <boost/unordered_map.hpp>
#include <glm/glm.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/matrix_decompose.hpp>

#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

#include <AssetManager.h>
#include <Memory/fmemory.h>
#include "EntityInterface.h"
#include <Log.h>

class Scene
{
private:
	boost::unordered_map<std::string, Mesh*> m_meshes;
	boost::unordered_map<std::string, Material*> m_materials;
	boost::container::vector<Entity, fmemory::STLAllocator<Entity>> m_entities;

	void LoadMesh(const std::string& meshPath);
	void LoadMaterial(const std::string& matPath);
public:
	void LoadScene(const char* sceneFilePath);
	void SaveScene(const char* sceneFilePath);
	void CloseScene();
};

#endif