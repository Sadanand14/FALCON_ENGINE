#include "AssetManager.h"
#include "PipeLine/Mesh.h"
#include "Font.h"
#include <Log.h>
#include <filesystem>
#include <algorithm>
#include "System/Types.h"

TextureType AssetManager::m_lastTextureType;
Mesh* AssetManager::m_cubeMesh = nullptr;
Shader* AssetManager::m_cubeShader = nullptr;
Shader* AssetManager::m_irradianceShader = nullptr;
Shader* AssetManager::m_prefilterShader = nullptr;

boost::mutex AssetManager::AssetMtx;

boost::unordered_map<std::string, Mesh*> AssetManager::m_meshes;
boost::unordered_map<std::string, Material*> AssetManager::m_materials;
boost::unordered_map<std::string, Font*> AssetManager::m_fonts;

void AssetManager::CreateDefaultFont()
{
	nk_font_atlas atlas;
	nk_font_atlas_init_default(&atlas);
	nk_font_atlas_begin(&atlas);

	nk_font* font = nk_font_atlas_add_default(&atlas, 13, 0);

	Font* f = fmemory::fnew<Font>(atlas, font);

	AssetMtx.lock();
	m_fonts["default"] = f;
	AssetMtx.unlock();
}

/**
 * Loads a mesh into the scene
 * @param meshPath - The path to the mesh json file
 */
Mesh* AssetManager::GetMesh(const std::string& path)
{
	//return if mesh is already loaded
	auto iterator = m_meshes.find(path);
	if (iterator != m_meshes.end())
		return iterator->second;

	//Get file data
	char* json = nullptr;
	int32_t size;
	std::ifstream jsonFile(path, std::ios::in | std::ios::ate | std::ios::binary);
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
	//std::string const& temp = path;
	std::string meshPath = doc["path"].GetString();
	Mesh* mesh = LoadModel(meshPath);

	bool transparent = doc["transparent"].GetBool();
	mesh->SetTransparent(transparent);

	mesh->SetMaterial(GetMaterial(doc["material"].GetString()));
	mesh->PreallocMatrixAmount(doc["instances"].GetInt());

	AssetMtx.lock();
	m_meshes[path] = mesh;
	AssetMtx.unlock();
	return mesh;
}

/**
 * Loads a sky into the scene
 * @param path - The path to the sky json file
 */
Mesh* AssetManager::GetSky(const std::string& path)
{
	//return if mesh is already loaded
	auto iterator = m_meshes.find(path);
	if (iterator != m_meshes.end())
		return iterator->second;

	//Get file data
	char* json = nullptr;
	int32_t size;
	std::ifstream jsonFile(path, std::ios::in | std::ios::ate | std::ios::binary);
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
	//std::string const& temp = path;
	std::string meshPath = doc["path"].GetString();
	Mesh* mesh = LoadModel(meshPath);

	bool transparent = doc["transparent"].GetBool();
	mesh->SetTransparent(transparent);

	mesh->SetMaterial(GetSkyMaterial(doc["material"].GetString()));
	mesh->PreallocMatrixAmount(doc["instances"].GetInt());

	AssetMtx.lock();
	m_meshes[path] = mesh;
	AssetMtx.unlock();
	return mesh;
}

/**
* AssetManager function that creates a terrain based on the heightmap data provided in the .raw file.
@param[in] Path to the .raw file.
*/
Mesh* AssetManager::LoadTerrain(const std::string& path)
{
	//Get file data
	char* json = nullptr;
	int32_t size;
	std::ifstream jsonFile(path, std::ios::in | std::ios::ate | std::ios::binary);
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


	if (doc.HasMember("heightMap"))
	{
		const char* rawPath = doc["heightMap"].GetString();
		//Defining variables for use
		int error, count;
		unsigned int resolution = 512;
		++resolution;
		unsigned int numVerts = resolution * resolution;
		unsigned int numIndicies = (resolution - 1) * (resolution - 1) * 6;

		//creating arrays to store data
		std::vector<u32, fmemory::STLAllocator<u32>> heightArray;
		heightArray.resize(numVerts);
		//u32* heightArray = new u32[numVerts];
		boost::container::vector<Vertex, fmemory::STLAllocator<Vertex>> terrainVertices;
		terrainVertices.resize(numVerts);
		boost::container::vector<u32, fmemory::STLAllocator<u32>> terrainIndices;
		terrainIndices.resize(numIndicies);

		boost::container::vector<u32, fmemory::STLAllocator<u32>> terrainOffsets;
		terrainOffsets.push_back(0);

		FILE* file;

#ifdef FL_PLATFORM_WINDOWS
		error = fopen_s(&file, rawPath, "rb");
		if (error != 0) FL_ENGINE_ERROR("Couldn't pen provided .raw file!");
#elif FL_PLATFORM_UNIX
		file = fopen(rawPath, "rb");
#endif
		count = fread(heightArray.data(), sizeof(unsigned short), numVerts, file);

		//std::cout << "height array values" << heightArray[10] << std::endl;

		error = fclose(file);
		if (error != 0) FL_ENGINE_ERROR("Error closing the file!");

		for (unsigned int i = 0; i < resolution; ++i)
		{
			for (unsigned int j = 0; j < resolution; ++j)
			{
				Vertex V = Vertex();
				//std::cout << i << "," << j << "\n";
				V.Position = glm::vec3(i, (float)heightArray[i * resolution + j] / 450000000, j);
				V.TexCoords = glm::vec2(i / 10.0f, j / 10.0f);
				V.Normal = glm::vec3(0, 1, 0);
				V.Tangent = glm::vec3(0, 0, 0);
				V.Bitangent = glm::vec3(0, 0, 0);
				terrainVertices[i * resolution + j] = V;
			}
		}

		int Index = 0;
		for (unsigned int i = 0; i < resolution - 1; ++i)
		{
			for (unsigned int j = 0; j < resolution - 1; ++j)
			{
				terrainIndices[Index++] = i * resolution + j;
				terrainIndices[Index++] = i * resolution + j + 1;
				terrainIndices[Index++] = (i + 1) * resolution + j;
				terrainIndices[Index++] = i * resolution + j + 1;
				terrainIndices[Index++] = (i + 1) * resolution + j + 1;
				terrainIndices[Index++] = (i + 1) * resolution + j;
			}
		}

		Mesh* newmesh = fmemory::fnew<Mesh>();
		newmesh->m_vertexArray=terrainVertices;
		//std::copy(terrainVertices.begin(), terrainVertices.end(), newmesh->m_vertexArray);

		newmesh->m_indexArray=terrainIndices;
		//std::copy(terrainIndices.begin(), terrainIndices.end(), newmesh->m_indexArray);

		newmesh->m_indexOffsets = terrainOffsets;
		//std::copy(terrainOffsets.begin(), terrainOffsets.end(), newmesh->m_indexOffsets);

		if (doc.HasMember("material"))
			newmesh->SetMaterial(GetMaterial(doc["material"].GetString()));

		if (doc.HasMember("transparent")) newmesh->SetTransparent(doc["transparent"].GetBool());

		if (doc.HasMember("instances")) newmesh->PreallocMatrixAmount(doc["instances"].GetInt());

		newmesh->Setup();
		AssetMtx.lock();
		m_meshes[path] = newmesh;
		AssetMtx.unlock();
		return newmesh;
	}
	return nullptr;
}

/**
 * Loads a material into the scene
 * @param matPath - The path to the material json file
 */
Material* AssetManager::GetMaterial(const std::string& path)
{
	AssetMtx.lock();
	//return if material is already loaded
	auto mat = m_materials.find(path);
	AssetMtx.unlock();
	if (mat != m_materials.end())
		return mat->second;

	//Load material if it doesn't exist
	Material* material = LoadMaterial(path);
	AssetMtx.lock();
	m_materials[path] = material;
	AssetMtx.unlock();
	return material;

}

/**
 * Loads a material into the scene
 * @param matPath - The path to the material json file
 */
Material* AssetManager::GetSkyMaterial(const std::string& path)
{
	AssetMtx.lock();
	//return if material is already loaded
	auto mat = m_materials.find(path);
	AssetMtx.unlock();
	if (mat != m_materials.end())
		return mat->second;

	//Load material if it doesn't exist
	Material* material = LoadSkyMaterial(path);
	AssetMtx.lock();
	m_materials[path] = material;
	AssetMtx.unlock();
	return material;

}

/**
*This function intializes the loading of a model using the ASSIMP Library.
*
*@param[in] File Path
*@param[out] A mesh type data structure of the model.
*/
Mesh* AssetManager::LoadModel(std::string const& path)
{
	// Read File (Assimp)
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
	// Error Check
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		FL_ENGINE_ERROR("ERROR::ASSIMP::{0}", importer.GetErrorString());
		return nullptr;
	}

	Mesh* newmesh = fmemory::fnew<Mesh>();

	//FL_ENGINE_ERROR("LOADMODEL!!!");
	// Process rootnode
	ProcessNode(scene->mRootNode, scene, newmesh);
	newmesh->Setup();
	return newmesh;
}


/**
*This function loads a texture file using the STBI library.
*
*@param[in] File Location
*@param[in] The number of channels
*@param[out] Texture ID
*/
u32 AssetManager::LoadTexture(std::string const& path, int channels)
{
	std::string filename(path);

	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	stbi_set_flip_vertically_on_load(false);

	unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, channels);
	m_lastTextureType = TextureType::TEXTURE2D;

	if(channels != 0)
		nrComponents = channels;

	if (data)
	{
		GLenum format = 0;
		switch (nrComponents) {
		case 1:
			format = GL_RED;
			break;
		case 2:
			format = GL_RG;
			break;
		case 3:
			format = GL_RGB;
			break;
		case 4:
		default:
			format = GL_RGBA;
			break;
		}

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		FL_ENGINE_ERROR("ERROR: Texture failed to load at {0} ", path);
		stbi_image_free(data);
	}

	return textureID;
}

/**
* AssetManager Function Responsible for loading Material data from the provided json file path.
* @param[in] A path to the json file defining the type of data to be held by that Material instance.
*/
Material* AssetManager::LoadMaterial(std::string const& path)
{
	//Get file data
	char* json = nullptr;
	int32_t size;
	std::ifstream jsonFile(path, std::ios::in | std::ios::ate | std::ios::binary);
	if (jsonFile.is_open()) {
		size = jsonFile.tellg();
		jsonFile.seekg(std::ios::beg);
		json = fmemory::fnew_arr<char>(size + 1);
		//json = new char[size + 1];
		jsonFile.read(json, size);
		json[size] = 0;
		jsonFile.close();
	}

	//Start json doc
	rapidjson::Document doc;
	doc.Parse(json);
	fmemory::fdelete<char>(json);

	////TODO: Change this to actually load a material using json and remove tmp things
	Material* mat = fmemory::fnew<Material>();

	if (doc.HasMember("Vshader") && doc.HasMember("Fshader"))
	{
		std::string Vshader = doc["Vshader"].GetString();
		std::string Fshader = doc["Fshader"].GetString();
		Shader* shader = fmemory::fnew<Shader>(Vshader.c_str(), Fshader.c_str());
		mat->SetShader(shader);
	}

	if (doc.HasMember("albedo"))
	{
		mat->m_albedoTex.textureID = LoadTexture(doc["albedo"].GetString());
		mat->m_albedoTex.type = m_lastTextureType;
	}

	else
	{
		uint8_t albedo[3] = { 255, 255, 255 };

		//Create some default textures
		mat->m_albedoTex.type = TextureType::TEXTURE2D;

		glGenTextures(1, &mat->m_albedoTex.textureID);
		glBindTexture(GL_TEXTURE_2D, mat->m_albedoTex.textureID);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, 1, 1, 0, GL_RGB, GL_UNSIGNED_BYTE, &albedo);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	if (doc.HasMember("roughness"))
	{
		mat->m_roughnessTex.textureID = LoadTexture(doc["roughness"].GetString(), 1);
		mat->m_roughnessTex.type = m_lastTextureType;
	}

	else
	{
		uint8_t rough = 255;
		mat->m_roughnessTex.type = TextureType::TEXTURE2D;

		glGenTextures(1, &mat->m_roughnessTex.textureID);
		glBindTexture(GL_TEXTURE_2D, mat->m_roughnessTex.textureID);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, 1, 1, 0, GL_RED, GL_UNSIGNED_BYTE, &rough);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	if (doc.HasMember("normal"))
	{
		mat->m_normalTex.textureID = LoadTexture(doc["normal"].GetString());
		mat->m_normalTex.type = m_lastTextureType;
	}

	else
	{
		uint8_t normal[3] = { 128, 128, 255 };
		mat->m_normalTex.type = TextureType::TEXTURE2D;

		glGenTextures(1, &mat->m_normalTex.textureID);
		glBindTexture(GL_TEXTURE_2D, mat->m_normalTex.textureID);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, 1, 1, 0, GL_RGB, GL_UNSIGNED_BYTE, &normal);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	if (doc.HasMember("metallic"))
	{
		mat->m_metallicTex.textureID = LoadTexture(doc["metallic"].GetString(), 1);
		mat->m_metallicTex.type = m_lastTextureType;
	}

	else
	{
		uint8_t metallic = 0;
		mat->m_metallicTex.type = TextureType::TEXTURE2D;

		glGenTextures(1, &mat->m_metallicTex.textureID);
		glBindTexture(GL_TEXTURE_2D, mat->m_metallicTex.textureID);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, 1, 1, 0, GL_RED, GL_UNSIGNED_BYTE, &metallic);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	if (doc.HasMember("ao"))
	{
		mat->m_aoTex.textureID = LoadTexture(doc["ao"].GetString(), 1);
		mat->m_aoTex.type = m_lastTextureType;
	}

	else
	{
		uint8_t ao = 255;
		mat->m_aoTex.type = TextureType::TEXTURE2D;

		glGenTextures(1, &mat->m_aoTex.textureID);
		glBindTexture(GL_TEXTURE_2D, mat->m_aoTex.textureID);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, 1, 1, 0, GL_RED, GL_UNSIGNED_BYTE, &ao);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	return mat;
}

/**
 * AssetManager Function Responsible for loading Material data from the provided json file path.
 * @param[in] A path to the json file defining the type of data to be held by that Material instance.
 */
Material* AssetManager::LoadSkyMaterial(std::string const& path)
{
	//Get file data
	char* json = nullptr;
	int32_t size;
	std::ifstream jsonFile(path, std::ios::in | std::ios::ate | std::ios::binary);
	if (jsonFile.is_open()) {
		size = jsonFile.tellg();
		jsonFile.seekg(std::ios::beg);
		json = fmemory::fnew_arr<char>(size + 1);
		//json = new char[size + 1];
		jsonFile.read(json, size);
		json[size] = 0;
		jsonFile.close();
	}

	//Start json doc
	rapidjson::Document doc;
	doc.Parse(json);
	fmemory::fdelete<char>(json);

	////TODO: Change this to actually load a material using json and remove tmp things
	Material* mat = fmemory::fnew<Material>();

	if (doc.HasMember("Vshader") && doc.HasMember("Fshader"))
	{
		std::string Vshader = doc["Vshader"].GetString();
		std::string Fshader = doc["Fshader"].GetString();
		Shader* shader = fmemory::fnew<Shader>(Vshader.c_str(), Fshader.c_str());
		mat->SetShader(shader);
	}

	if (doc.HasMember("albedo"))
	{
		std::string filename(doc["albedo"].GetString());

		int width, height, nrComponents;
		std::string temp = filename.substr(filename.size() - 3, 3);

		if (temp == "hdr")
		{
			stbi_set_flip_vertically_on_load(true);
			int width, height, nrComponents;
			float* data = stbi_loadf(filename.c_str(), &width, &height, &nrComponents, 0);
			unsigned int hdrTexture;
			if (data)
			{
				glGenTextures(1, &hdrTexture);
				glBindTexture(GL_TEXTURE_2D, hdrTexture);
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, data);

				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

				stbi_image_free(data);
			}
			else
			{
				FL_ENGINE_ERROR("ERROR: HDR Texture failed to load at {0} ", path);
				stbi_image_free(data);
			}

			HDRtoCubemap(hdrTexture, &mat->m_albedoTex.textureID, &mat->m_normalTex.textureID, &mat->m_roughnessTex.textureID);
		}

		mat->m_albedoTex.type = TextureType::CUBEMAP;
		mat->m_normalTex.type = TextureType::CUBEMAP;
		mat->m_roughnessTex.type = TextureType::CUBEMAP;
	}

	return mat;
}


//Private Functions
/**
*This function iterates through the scene node provided by Assimp and Retrives all the mesh data.
*
*@param[in] An aiNode* type pointer(defined in assimp Library)
*@param[in] An aiScene* type pointer(defined in assimp Library)
*@param[in] A new Mesh pointer to store all the mesh data into.
*/
void AssetManager::ProcessNode(aiNode* node, const aiScene* scene, Mesh* newMesh)
{
	//FL_ENGINE_ERROR("ProcessNode!!!");
	// Process each mesh located at the current node.
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		ProcessMesh(mesh, newMesh);
	}
	//Process children nodes.


	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		ProcessNode(node->mChildren[i], scene, newMesh);
	}
}

/**
*This function retrieves the mesh data and stores in the provided mesh location.
*
*@param[in] An aiNode* type pointer(defined in assimp Library)
*@param[in] A new Mesh pointer to store all the mesh data into.
*/
void AssetManager::ProcessMesh(aiMesh* mesh, Mesh* newMesh)
{
	//FL_ENGINE_ERROR("ProcessMesh!!!");
	// Data to load
	size_t indexOffset = 0;

	bool uvs = false;
	bool normals = false;
	bool tanBitan = false;

	if (mesh->HasTextureCoords(0))
		uvs = true;
	if (mesh->HasNormals())
		normals = true;
	if (mesh->HasTangentsAndBitangents())
		tanBitan = true;


	// Walk through each of the mesh's vertices.
	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;
		//Position
		vertex.Position = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);

		// Texture Coordinates
		if (uvs)
		{
			// a vertex can contain up to 8 different texture coordinates.
			//To-Do: Add support for models that use more than one set of texture coordinates.
			vertex.TexCoords = glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
		}
		else
			vertex.TexCoords = glm::vec2(0.0f, 0.0f);

		if (normals)
		{
			// Normals
			vertex.Normal = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
		}
		else
			vertex.Normal = glm::vec3(0.0f, 0.0f, 0.0f);


		if (tanBitan) {
			// Tangent
			vertex.Tangent = glm::vec3(mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z);

			// Bitangent
			vertex.Bitangent = glm::vec3(mesh->mBitangents[i].x, mesh->mBitangents[i].y, mesh->mBitangents[i].z);
		}

		else {

			vertex.Tangent = glm::vec3(0.0f, 0.0f, 0.0f);
			vertex.Bitangent = glm::vec3(0.0f, 0.0f, 0.0f);
		}

		newMesh->m_vertexArray.push_back(vertex);
	}

	newMesh->m_indexOffsets.push_back(newMesh->m_indexArray.size());

	// now wak through each of the mesh's faces
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];

		//FL_ENGINE_ERROR("ProcessNode : {0} : {1}", face.mNumIndices);
		// retrieve all indices of the face and store them in the indices vector
		for (unsigned int j = 0; j < face.mNumIndices; j++)
			newMesh->m_indexArray.push_back(face.mIndices[j]);
	}
}

/**
* Private AssetManager function to convert loaded HDR textures into cubemap textures.
@param[in] The handle to the HDR texture that needs to be converted (GLuint).
*/
void AssetManager::HDRtoCubemap(GLuint hdrTex, u32* cubeMap, u32* irradianceMap, u32* prefilterMap)
{
	unsigned int captureFBO, captureRBO;
	glGenFramebuffers(1, &captureFBO);
	glGenRenderbuffers(1, &captureRBO);
	glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
	glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 512, 512);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, captureRBO);

	//Capture views
	glm::mat4 captureProjection = (glm::mat4)glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
	glm::mat4 captureViews[] =
	{
	   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
	   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
	   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
	   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
	   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
	   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
	};

	/////////////////////
	//// Generate cubemap
	if (m_cubeShader == nullptr)m_cubeShader = fmemory::fnew<Shader>("../Falcon/Rendering/Shader/VS_CubeMapShader.vert", "../Falcon/Rendering/Shader/PS_CubeMapShader.frag");
	m_cubeShader->UseShader();


	if (m_cubeMesh == nullptr)m_cubeMesh = LoadModel("../Assets/Models/Skybox/cube.obj");

	m_cubeMesh->Bind();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, hdrTex);

	// convert HDR equirectangular environment map to cubemap equivalent
	m_cubeShader->SetInt("equirectangularMap", 0);
	m_cubeShader->SetMat4("projection", captureProjection);

	glGenTextures(1, cubeMap);
	glBindTexture(GL_TEXTURE_CUBE_MAP, *cubeMap);
	for (unsigned int i = 0; i < 6; ++i)
	{
		// note that we store each face with 16 bit floating point values
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F,
					 512, 512, 0, GL_RGB, GL_FLOAT, nullptr);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	glViewport(0, 0, 512, 512); // don't forget to configure the viewport to the capture dimensions.
	glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
	for (unsigned int i = 0; i < 6; ++i)
	{
		/*if (i == 0)
		{*/
			m_cubeShader->SetMat4("view", captureViews[i]);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
				GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, *cubeMap, 0);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			//TODO DRAW A 1x1 CUBE
			glDrawElements(GL_TRIANGLES, m_cubeMesh->m_indexArray.size(), GL_UNSIGNED_INT, 0);
			//renderCube(); // renders a 1x1 cube
		//}
	}
	glBindTexture(GL_TEXTURE_2D, 0);

	/////////////////////
	//// Generate irradiance map
	if (m_irradianceShader == nullptr) m_irradianceShader = fmemory::fnew<Shader>("../Falcon/Rendering/Shader/VS_CubeMapShader.vert", "../Falcon/Rendering/Shader/irradianceMap.frag");
	m_irradianceShader->UseShader();
	m_irradianceShader->SetMat4("projection", captureProjection);

	glGenTextures(1, irradianceMap);
	glBindTexture(GL_TEXTURE_CUBE_MAP, *irradianceMap);
	for (unsigned int i = 0; i < 6; ++i)
	{
		// note that we store each face with 16 bit floating point values
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F,
					 32, 32, 0, GL_RGB, GL_FLOAT, nullptr);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glViewport(0, 0, 32, 32); // don't forget to configure the viewport to the capture dimensions.
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 32, 32);
	glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, *cubeMap);
	m_irradianceShader->SetInt("cubeMap", 0);

	for (unsigned int i = 0; i < 6; ++i)
	{
		/*if (i == 0)
		 *{*/
		m_irradianceShader->SetMat4("view", captureViews[i]);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
							   GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, *irradianceMap, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//TODO DRAW A 1x1 CUBE
		glDrawElements(GL_TRIANGLES, m_cubeMesh->m_indexArray.size(), GL_UNSIGNED_INT, 0);
		//renderCube(); // renders a 1x1 cube
		//}
	}
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	/////////////////////
	//// Generate prefilter map
	if (m_prefilterShader == nullptr) m_prefilterShader = fmemory::fnew<Shader>("../Falcon/Rendering/Shader/VS_CubeMapShader.vert", "../Falcon/Rendering/Shader/prefilterMap.frag");
	m_prefilterShader->UseShader();

	m_prefilterShader->SetMat4("projection", captureProjection);
	m_prefilterShader->SetVec2("res", 512, 512);

	glGenTextures(1, prefilterMap);
	glBindTexture(GL_TEXTURE_CUBE_MAP, *prefilterMap);
	for (unsigned int i = 0; i < 6; ++i)
	{
		// note that we store each face with 16 bit floating point values
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F,
					 128, 128, 0, GL_RGB, GL_FLOAT, nullptr);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
	glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, *cubeMap);
	m_prefilterShader->SetInt("cubeMap", 0);

	for (uint32_t mip = 0; mip < 5; mip++) {
		uint32_t mipSize = 128 * std::pow(0.5f, mip);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, mipSize, mipSize);
		glViewport(0, 0, mipSize, mipSize); // don't forget to configure the viewport to the capture dimensions.
		float roughness = (float)mip / 4.0f;
		m_prefilterShader->SetFloat("roughness", roughness);

		//For each view: set the ubo matrices, attach the texture, clear the fb, run the prefilter shader
		for (uint32_t i = 0; i < 6; i++) {
			m_prefilterShader->SetMat4("view", captureViews[i]);

			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
							   GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, *prefilterMap, mip);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glDrawElements(GL_TRIANGLES, m_cubeMesh->m_indexArray.size(), GL_UNSIGNED_INT, 0);
		}
	}
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, 1280, 720);

	glDeleteFramebuffers(1, &captureFBO);
	glDeleteRenderbuffers(1, &captureRBO);
}

Font* AssetManager::GetFont(std::string const &path)
{
	//Return the font if it is already loaded
	auto fnt = m_fonts.find(path);
	if (fnt != m_fonts.end())
		return fnt->second;

	//Load material if it doesn't exist
	Font* font = LoadFont(path);
	AssetMtx.lock();
	m_fonts[path] = font;
	AssetMtx.unlock();
	return font;
}

Font* AssetManager::LoadFont(const std::string& path)
{
	struct nk_font_config cfg = nk_font_config(0);
	cfg.oversample_h = 3;
	cfg.oversample_v = 2;

	nk_font_atlas atlas;
	nk_font* font;

	nk_font_atlas_init_default(&atlas);
	nk_font_atlas_begin(&atlas);
	font = nk_font_atlas_add_from_file(&atlas, path.c_str(), 14.0f, &cfg);

	Font* fnt = fmemory::fnew<Font>(atlas, font);
	return fnt;
}

/**
* Assetmanager member function responsible for cleaning up all resources held by the AssetManager
*/
void AssetManager::Clean()
{
	if (m_cubeMesh != nullptr) fmemory::fdelete<>(m_cubeMesh);
	if (m_cubeShader != nullptr) fmemory::fdelete<>(m_cubeShader);
	for (auto it = m_meshes.begin(); it != m_meshes.end(); it++)
	{
		fmemory::fdelete<Mesh>(it->second);
	}

	for (auto it = m_materials.begin(); it != m_materials.end(); it++)
	{
		fmemory::fdelete<Material>(it->second);
	}

	for (auto it = m_fonts.begin(); it != m_fonts.end(); it++)
	{
		fmemory::fdelete<Font>(it->second);
	}
}

//GLuint AssetManager::texture_loadDDS(const char* path) {
//	// lay out variables to be used
//	unsigned char* header;
//
//	unsigned int width;
//	unsigned int height;
//	unsigned int mipMapCount;
//
//	unsigned int blockSize;
//	unsigned int format;
//
//	unsigned int w;
//	unsigned int h;
//
//	unsigned char* buffer = 0;
//
//	GLuint tid = 0;
//
//	// open the DDS file for binary reading and get file size
//	FILE* f;
//
//	if (fopen_s(&f, path, "rb") == 0)
//		return 0;
//	fseek(f, 0, SEEK_END);
//	long file_size = ftell(f);
//	fseek(f, 0, SEEK_SET);
//
//	// allocate new unsigned char space with 4 (file code) + 124 (header size) bytes
//	// read in 128 bytes from the file
//	header = new unsigned char(128);
//	fread(header, 1, 128, f);
//
//	// compare the `DDS ` signature
//	if (memcmp(header, "DDS ", 4) != 0)
//	{
//		free(buffer);
//		free(header);
//		fclose(f);
//		return tid;
//	}
//
//
//	// extract height, width, and amount of mipmaps - yes it is stored height then width
//	height = (header[12]) | (header[13] << 8) | (header[14] << 16) | (header[15] << 24);
//	width = (header[16]) | (header[17] << 8) | (header[18] << 16) | (header[19] << 24);
//	mipMapCount = (header[28]) | (header[29] << 8) | (header[30] << 16) | (header[31] << 24);
//
//	// figure out what format to use for what fourCC file type it is
//	// block size is about physical chunk storage of compressed data in file (important)
//	if (header[84] == 'D') {
//		switch (header[87]) {
//		case '1': // DXT1
//			format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
//			blockSize = 8;
//			break;
//		case '3': // DXT3
//			format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
//			blockSize = 16;
//			break;
//		case '5': // DXT5
//			format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
//			blockSize = 16;
//			break;
//		case '0': // DX10
//			// unsupported, else will error
//			// as it adds sizeof(struct DDS_HEADER_DXT10) between pixels
//			// so, buffer = malloc((file_size - 128) - sizeof(struct DDS_HEADER_DXT10));
//		default:free(buffer);
//			free(header);
//			fclose(f);
//			return tid;
//
//		}
//	}
//	else // BC4U/BC4S/ATI2/BC55/R8G8_B8G8/G8R8_G8B8/UYVY-packed/YUY2-packed unsupported
//	{
//		free(buffer);
//		free(header);
//		fclose(f);
//		return tid;
//	}
//
//	// allocate new unsigned char space with file_size - (file_code + header_size) magnitude
//	// read rest of file
//	buffer = new unsigned char(file_size - 128);
//	if (buffer == 0)
//	{
//		free(buffer);
//		free(header);
//		fclose(f);
//		return tid;
//	}
//	fread(buffer, 1, file_size, f);
//
//	// prepare new incomplete texture
//	glGenTextures(1, &tid);
//	if (tid == 0)
//	{
//		free(buffer);
//		free(header);
//		fclose(f);
//		return tid;
//	}
//
//	// bind the texture
//	// make it complete by specifying all needed parameters and ensuring all mipmaps are filled
//	glBindTexture(GL_TEXTURE_2D, tid);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, mipMapCount - 1); // opengl likes array length of mipmaps
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); // don't forget to enable mipmaping
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//
//	// prepare some variables
//	unsigned int offset = 0;
//	unsigned int size = 0;
//	w = width;
//	h = height;
//
//	// loop through sending block at a time with the magic formula
//	// upload to opengl properly, note the offset transverses the pointer
//	// assumes each mipmap is 1/2 the size of the previous mipmap
//	for (unsigned int i = 0; i < mipMapCount; i++) {
//		if (w == 0 || h == 0) { // discard any odd mipmaps 0x1 0x2 resolutions
//			mipMapCount--;
//			continue;
//		}
//		size = ((w + 3) / 4) * ((h + 3) / 4) * blockSize;
//		glCompressedTexImage2D(GL_TEXTURE_2D, i, format, w, h, 0, size, buffer + offset);
//		offset += size;
//		w = std::max((unsigned int)1, (unsigned int)w / 2);
//		h = std::max((unsigned int)1, (unsigned int)h / 2);
//	}
//	// discard any odd mipmaps, ensure a complete texture
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, mipMapCount - 1);
//	// unbind
//	glBindTexture(GL_TEXTURE_2D, 0);
//
//	// easy macro to get out quick and uniform (minus like 15 lines of bulk)
//exit:
//	free(buffer);
//	free(header);
//	fclose(f);
//	return tid;
//}