#include "EntityManager.h"

boost::unordered_map<std::string, Mesh*> EntityManager::m_meshes;
boost::unordered_map<std::string, Material*> EntityManager::m_materials;

/**
 * Loads a mesh into the scene
 * @param meshPath - The path to the mesh json file
 */
void EntityManager::LoadMesh(const std::string& meshPath)
{
	//Get file data
	char* json = nullptr;
	int32_t size;
	std::ifstream jsonFile(meshPath, std::ios::in | std::ios::ate | std::ios::binary);
	if (jsonFile.is_open()) {
		size = jsonFile.tellg();
		jsonFile.seekg(std::ios::beg);
		json = fmemory::fnew_arr<char>(size + 1);
		jsonFile.read(json, size);
		json[size] = 0;
		jsonFile.close();
	}

	//Start json doc
	rapidjson::Document doc;
	doc.Parse(json);
	fmemory::fdelete<char>(json);

	//Set the mesh path
	std::string path = doc["path"].GetString();
	Mesh* m = AssetManager::LoadModel(path);
	m->SetJsonPath(meshPath);
	m->SetPath(path);

	//Check if the material already exists
	auto mat = m_materials.find(doc["material"].GetString());

	//Load material if it doesn't exist
	if (mat == m_materials.end())
		LoadMaterial(doc["material"].GetString());

	//Set material and alloc the amount of instances for the matrices
	m->SetMaterial(m_materials[doc["material"].GetString()]);
	m->PreallocMatrixAmount(doc["instances"].GetInt());
	m_meshes[meshPath] = m;
}

/**
 * Loads a material into the scene
 * @param matPath - The path to the material json file
 */
void EntityManager::LoadMaterial(const std::string& matPath)
{
	Material* mat = AssetManager::LoadMaterial(matPath);
	mat->SetPath(matPath);
	m_materials[matPath] = mat;
}

/**
 * Loads a scene
 * @param sceneFilePath - The path to the scene file
 */


Entity* EntityManager::CreateEntity(const char* objTemplate, glm::vec3 pos, glm::quat rot, glm::vec3 scale)
{
	Entity* newEntity = nullptr;
	if (objTemplate != NULL)
	{
		newEntity = new Entity(pos, rot, scale);

		//Get file data
		char* json = nullptr;
		int32_t size;
		std::ifstream jsonFile(objTemplate, std::ios::in | std::ios::ate | std::ios::binary);
		if (jsonFile.is_open()) {
			size = jsonFile.tellg();
			jsonFile.seekg(std::ios::beg);
			json = fmemory::fnew_arr<char>(size + 1);
			jsonFile.read(json, size);
			json[size] = 0;
			jsonFile.close();
		}

		//Start json doc
		rapidjson::Document doc;
		doc.Parse(json);
		//fmemory::fdelete<char>(json);

		//Check if JSON file is not valid
		if (!doc.IsObject())
		{
			FL_ENGINE_ERROR("ERROR::{0} is not a valid JSON file", objTemplate);
			return nullptr;
		}

		//Add render component
		if (doc.HasMember("renderComponent"))
		{
			newEntity->AddComponent<RenderComponent>();

			//Get mesh
			const rapidjson::Value& mesh = doc["renderComponent"]["mesh"];

			auto m = m_meshes.find(mesh.GetString());

			if (m == m_meshes.end())
				LoadMesh(mesh.GetString());
			newEntity->GetComponent<RenderComponent>()->m_mesh = m_meshes[mesh.GetString()];
		}

		//TODO:: DO REST OF THE COMPONENT READINGS WHEN THE COMPONENTS BECOME AVAILABLE
	}

	newEntity->AddComponent<ParticleEmitterComponent>();
	LoadMaterial(std::string("../Assets/Textures/material.json"));
	newEntity->GetComponent<ParticleEmitterComponent>()->m_particle = fmemory::fnew<Particle>();
	newEntity->GetComponent<ParticleEmitterComponent>()->m_particle->Setup();
	newEntity->GetComponent<ParticleEmitterComponent>()->m_particle->PreallocParticleDataAmount(20);
	newEntity->GetComponent<ParticleEmitterComponent>()->m_particle->SetMaterial(m_materials["../Assets/Textures/material.json"]);

	return newEntity;
}

/**
 * Saves a scene
 * @param sceneFilePath - The path to the scene file
 */
void EntityManager::SaveScene(const char* sceneFilePath)
{
	////Save all the mesh files
	//for (auto it = m_meshes.begin(); it != m_meshes.end(); it++)
	//{
	//	//New doc
	//	rapidjson::Document meshDoc;
	//	meshDoc.SetObject();
	//	rapidjson::Document::AllocatorType& alloc = meshDoc.GetAllocator();

	//	//Get values
	//	rapidjson::Value path;
	//	path.SetString(rapidjson::StringRef(it->second->GetPath().c_str()));
	//	rapidjson::Value mat;
	//	mat.SetString(rapidjson::StringRef(it->second->GetMaterial()->GetPath().c_str()));
	//	rapidjson::Value inst(it->second->GetWorldMatrixAmount());

	//	//Add values to document
	//	meshDoc.AddMember("path", path, alloc);
	//	meshDoc.AddMember("material", mat, alloc);
	//	meshDoc.AddMember("instances", inst, alloc);

	//	//Convert scene to stringbuffer
	//	rapidjson::StringBuffer sb;
	//	rapidjson::Writer<rapidjson::StringBuffer> writer(sb);
	//	meshDoc.Accept(writer);

	//	//Write scene to file
	//	std::ofstream meshFile(it->first, std::ofstream::out | std::ios::binary);
	//	meshFile << sb.GetString();
	//	meshFile.close();
	//}

	//for (auto it = m_materials.begin(); it != m_materials.end(); it++)
	//{
	//	//New doc
	//	rapidjson::Document matDoc;
	//	matDoc.SetObject();
	//	rapidjson::Document::AllocatorType& alloc = matDoc.GetAllocator();

	//	rapidjson::Value albedo;
	//	albedo.SetString(rapidjson::StringRef(it->second->GetTexturePath(0).c_str()));
	//	rapidjson::Value rough;
	//	rough.SetString(rapidjson::StringRef(it->second->GetTexturePath(1).c_str()));
	//	rapidjson::Value norm;
	//	norm.SetString(rapidjson::StringRef(it->second->GetTexturePath(2).c_str()));
	//	rapidjson::Value metal;
	//	metal.SetString(rapidjson::StringRef(it->second->GetTexturePath(3).c_str()));
	//	rapidjson::Value ao;
	//	ao.SetString(rapidjson::StringRef(it->second->GetTexturePath(4).c_str()));


	//	matDoc.AddMember("albedo", albedo, alloc);
	//	matDoc.AddMember("roughness", rough, alloc);
	//	matDoc.AddMember("normal", norm, alloc);
	//	matDoc.AddMember("metallic", metal, alloc);
	//	matDoc.AddMember("ao", ao, alloc);

	//	//Convert scene to stringbuffer
	//	rapidjson::StringBuffer sb;
	//	rapidjson::Writer<rapidjson::StringBuffer> writer(sb);
	//	matDoc.Accept(writer);

	//	//Write scene to file
	//	std::ofstream matFile(it->first, std::ofstream::out | std::ios::binary);
	//	matFile << sb.GetString();
	//	matFile.close();
	//}

	////New doc
	//rapidjson::Document doc;
	//doc.SetObject();
	//rapidjson::Document::AllocatorType& alloc = doc.GetAllocator();

	////Array of entities
	//rapidjson::Value entityArray(rapidjson::kArrayType);
	//entityArray.Reserve(m_entities.size(), alloc);
	//for (u32 i = 0; i < m_entities.size(); i++)
	//{
	//	//New Entity
	//	rapidjson::Value entity(rapidjson::kObjectType);

	//	//Get position, rotation, and scale
	//	rapidjson::Value pos(rapidjson::kArrayType);
	//	for (rapidjson::SizeType j = 0; j < 3; j++)
	//	{
	//		pos.PushBack(m_entities[i].GetTransform()->GetPosition()[j], alloc);
	//	}
	//	entity.AddMember("pos", pos, alloc);

	//	rapidjson::Value rot(rapidjson::kArrayType);
	//	for (rapidjson::SizeType j = 0; j < 4; j++)
	//	{
	//		rot.PushBack(m_entities[i].GetTransform()->GetRotation()[j], alloc);
	//	}
	//	entity.AddMember("rot", rot, alloc);

	//	rapidjson::Value scale(rapidjson::kArrayType);
	//	for (rapidjson::SizeType j = 0; j < 3; j++)
	//	{
	//		scale.PushBack(m_entities[i].GetTransform()->GetScale()[j], alloc);
	//	}
	//	entity.AddMember("scale", scale, alloc);

	//	//Add render component and mesh
	//	if (m_entities[i].GetComponent<RenderComponent>())
	//	{
	//		rapidjson::Value renderComp(rapidjson::kObjectType);

	//		rapidjson::Value mesh;
	//		mesh.SetString(rapidjson::StringRef(m_entities[i].GetComponent<RenderComponent>()->m_mesh->GetJsonPath().c_str()));
	//		renderComp.AddMember("mesh", mesh, alloc);
	//		entity.AddMember("renderComponent", renderComp, alloc);
	//	}

	//	//TODO Check and add other components

	//	//Add children
	//	rapidjson::Value children(rapidjson::kArrayType);
	//	for (i32 j = 0; j < m_entities[i].GetChildren().size(); j++)
	//	{
	//		children.PushBack(m_entities[i].GetChild(j), alloc);
	//	}

	//	entity.AddMember("children", children, alloc);

	//	entityArray.PushBack(entity, alloc);
	//}


	////Add objects to doc
	//doc.AddMember("entities", entityArray, alloc);

	////Convert scene to stringbuffer
	//rapidjson::StringBuffer sb;
	//rapidjson::Writer<rapidjson::StringBuffer> writer(sb);
	//doc.Accept(writer);

	////Write scene to file
	//std::ofstream outFile(sceneFilePath, std::ofstream::out | std::ios::binary);
	//outFile << sb.GetString();
	//outFile.close();
}

/**
 * Cleans up resources in the scene
 */
void EntityManager::CloseScene()
{
	//for(auto it = m_meshes.begin(); it != m_meshes.end(); it++)
	//	fmemory::fdelete<Mesh>(it->second);
	//for(auto it = m_materials.begin(); it != m_meshes.end(); it++)
	//	fmemory::fdelete<Materials>(it->second);
}