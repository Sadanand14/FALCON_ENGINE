#include "AssetManager.h"
#include "PipeLine/Mesh.h"
#include <Log.h>
#include <filesystem>

boost::unordered_map<std::string, Mesh*> AssetManager::m_meshes;
boost::unordered_map<std::string, Material*> AssetManager::m_materials;


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

	m_meshes[path] = mesh;
	return mesh;
}

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
		unsigned int resolution = 1024;
		++resolution;
		unsigned int numVerts = resolution * resolution;
		unsigned int numIndicies = (resolution - 1) * (resolution - 1) * 6;

		//creating arrays to store data
		std::vector<u32, fmemory::STLAllocator<u32>> heightArray;
		heightArray.resize(numVerts);
		//u32* heightArray = new u32[numVerts];
		std::vector<Vertex, fmemory::STLAllocator<Vertex>> terrainVertices;
		terrainVertices.resize(numVerts);
		std::vector<u32, fmemory::STLAllocator<u32>> terrainIndices;
		terrainIndices.resize(numIndicies);

		std::vector<u32, fmemory::STLAllocator<u32>> terrainOffsets;
		terrainOffsets.push_back(0);

		FILE* file;
		error = fopen_s(&file, rawPath, "rb");
		if (error != 0) FL_ENGINE_ERROR("Couldn't pen provided .raw file!");
		count = fread(heightArray.data(), sizeof(unsigned short), numVerts, file);

		std::cout << "height array values" << heightArray[10] << std::endl;

		error = fclose(file);
		if (error != 0) FL_ENGINE_ERROR("Error closing the file!");

		for (unsigned int i = 0; i < resolution; ++i)
		{
			for (unsigned int j = 0; j < resolution; ++j)
			{
				Vertex V = Vertex();
				//std::cout << i << "," << j << "\n";
				V.Position = glm::vec3(i, (float)heightArray[i * resolution + j] / 90000000, j);
				V.TexCoords = glm::vec2(i / 10.0f, j / 10.0f);
				V.Normal = glm::vec3(0, 1, 0);
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
		newmesh->m_vertexCount = terrainVertices.size();
		newmesh->m_vertexArray = new Vertex[terrainVertices.size()];//fmemory::fnew_arr<Vertex>(newmesh->m_vertexCount);
		std::copy(terrainVertices.begin(), terrainVertices.end(), newmesh->m_vertexArray);

		newmesh->m_indexCount = terrainIndices.size();
		newmesh->m_indexArray = new u32[terrainIndices.size()];//fmemory::fnew_arr<u32>(newmesh->m_indexCount);
		std::copy(terrainIndices.begin(), terrainIndices.end(), newmesh->m_indexArray);

		newmesh->m_indexOffsetCount = terrainOffsets.size();
		newmesh->m_indexOffsets = new u32[terrainOffsets.size()];//fmemory::fnew_arr<u32>(newmesh->m_indexOffsetCount);
		std::copy(terrainOffsets.begin(), terrainOffsets.end(), newmesh->m_indexOffsets);

		if (doc.HasMember("material")) newmesh->SetMaterial(GetMaterial(doc["material"].GetString()));

		if (doc.HasMember("transparent")) newmesh->SetTransparent(doc["transparent"].GetBool());
			
		if (doc.HasMember("instances")) newmesh->PreallocMatrixAmount(doc["instances"].GetInt());

		newmesh->Setup();
		m_meshes[path] = newmesh;
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
	//return if material is already loaded
	auto mat = m_materials.find(path);
	if (mat != m_materials.end())
		return mat->second;

	//Load material if it doesn't exist
	Material* material = LoadMaterial(path);
	m_materials[path] = material;
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

	boost::container::vector<Vertex> verts;
	boost::container::vector<uint32_t> inds;
	boost::container::vector<uint32_t> indOffsets;

	// Process rootnode
	ProcessNode(scene->mRootNode, scene, verts, inds, indOffsets);

	newmesh->m_vertexCount = verts.size();
	newmesh->m_vertexArray = new Vertex[newmesh->m_vertexCount];//fmemory::fnew_arr<Vertex>(newmesh->m_vertexCount);
	std::copy(verts.begin(), verts.end(), newmesh->m_vertexArray);

	newmesh->m_indexCount = inds.size();
	newmesh->m_indexArray = new u32[newmesh->m_indexCount];//fmemory::fnew_arr<u32>(newmesh->m_indexCount);
	std::copy(inds.begin(), inds.end(), newmesh->m_indexArray);

	newmesh->m_indexOffsetCount = indOffsets.size();
	newmesh->m_indexOffsets = new u32[newmesh->m_indexOffsetCount];//fmemory::fnew_arr<u32>(newmesh->m_indexOffsetCount);
	std::copy(indOffsets.begin(), indOffsets.end(), newmesh->m_indexOffsets);

	newmesh->Setup();
	return newmesh;
}


/**
*This function loads a texture file using the STBI library.
*
*@param[in] File Location
*@param[out] Texture ID
*/
u32 AssetManager::LoadTexture(std::string const& path)
{
	std::string filename(path);

	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
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
*
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

	//TODO: Change this to actually load a material using json and remove tmp things
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
		//mat->m_albedoTex.type = m_lastTextureType;
	}

	if (doc.HasMember("roughness"))
	{
		mat->m_roughnessTex.textureID = LoadTexture(doc["roughness"].GetString());
		//mat->m_roughnessTex.type = m_lastTextureType;
	}

	if (doc.HasMember("normal"))
	{
		mat->m_normalTex.textureID = LoadTexture(doc["normal"].GetString());
		//mat->m_normalTex.type = m_lastTextureType;
	}

	if (doc.HasMember("metallic"))
	{
		mat->m_metallicTex.textureID = LoadTexture(doc["metallic"].GetString());
		//mat->m_metallicTex.type = m_lastTextureType;
	}

	if (doc.HasMember("ao"))
	{
		mat->m_aoTex.textureID = LoadTexture(doc["ao"].GetString());
		//mat->m_aoTex.type = m_lastTextureType;
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
void AssetManager::ProcessNode(aiNode* node, const aiScene* scene, boost::container::vector<Vertex>& verts, boost::container::vector<uint32_t>& inds, boost::container::vector<uint32_t>& indOffsets)
{
	// Process each mesh located at the current node.
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		ProcessMesh(mesh, verts, inds, indOffsets);
	}
	//Process children nodes.
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		ProcessNode(node->mChildren[i], scene, verts, inds, indOffsets);
	}
}

/**
*This function retrieves the mesh data and stores in the provided mesh location.
*
*@param[in] An aiNode* type pointer(defined in assimp Library)
*@param[in] A new Mesh pointer to store all the mesh data into.
*/
void AssetManager::ProcessMesh(aiMesh* mesh, boost::container::vector<Vertex>& verts, boost::container::vector<uint32_t>& inds, boost::container::vector<uint32_t>& indOffsets)
{
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

		verts.push_back(vertex);
	}

	indOffsets.push_back(inds.size());

	// now wak through each of the mesh's faces
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		// retrieve all indices of the face and store them in the indices vector
		for (unsigned int j = 0; j < face.mNumIndices; j++)
			inds.push_back(face.mIndices[j]);
	}
}

void AssetManager::Clean()
{
	for (auto it = m_meshes.begin(); it != m_meshes.end(); it++)
	{
		fmemory::fdelete<Mesh>(it->second);
	}

	for (auto it = m_materials.begin(); it != m_materials.end(); it++)
	{
		fmemory::fdelete<Material>(it->second);
	}
}