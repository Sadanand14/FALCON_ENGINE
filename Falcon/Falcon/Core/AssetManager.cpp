#include "AssetManager.h"

#include "Log.h"


// Pass filepath to 3D model
AssetManager::AssetManager(std::string const& path, bool gamma /*false*/)
{
	LoadModel(path);
	FL_ENGINE_INFO("INFO: Model loaded sussfully for {0}", path);
}

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

	//Experimental
	Mesh* newmesh = new Mesh();
	newmesh->m_vertexArray.clear();
	newmesh->m_indexArray.clear();
	////////////////


	// Process rootnode 
	ProcessNode(scene->mRootNode, scene, newmesh);
	newmesh->SetupMesh();
	return newmesh;
}

void AssetManager::ProcessNode(aiNode* node, const aiScene* scene, Mesh* newmesh)
{
	// Process each mesh located at the current node.
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		ProcessMesh(mesh, scene, newmesh);
		//m_meshes.push_back(ProcessMesh(mesh, scene, newmesh));
	}
	//Process children nodes.
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		ProcessNode(node->mChildren[i], scene, newmesh);
	}
}

void AssetManager::ProcessMesh(aiMesh* mesh, const aiScene* scene, Mesh* newmesh)
{
	// Data to load
	//unsigned int 
	size_t indexOffset = 0;
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;

	// Walk through each of the mesh's vertices.
	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;
		glm::vec3 vector;
		// Positions
		vector.x = mesh->mVertices[i].x;
		vector.y = mesh->mVertices[i].y;
		vector.z = mesh->mVertices[i].z;
		vertex.Position = vector;
		// Normals
		vector.x = mesh->mNormals[i].x;
		vector.y = mesh->mNormals[i].y;
		vector.z = mesh->mNormals[i].z;
		vertex.Normal = vector;
		// Texture Coordinates
		if (mesh->mTextureCoords[0])
		{
			glm::vec2 vec;
			// a vertex can contain up to 8 different texture coordinates. 
			//To-Do: Add support for models that use more than one set of texture coordinates.
			vec.x = mesh->mTextureCoords[0][i].x;
			vec.y = mesh->mTextureCoords[0][i].y;
			vertex.TexCoords = vec;
		}
		else
			vertex.TexCoords = glm::vec2(0.0f, 0.0f);
		// Tangent
		vector.x = mesh->mTangents[i].x;
		vector.y = mesh->mTangents[i].y;
		vector.z = mesh->mTangents[i].z;
		vertex.Tangent = vector;
		// Bitangent
		vector.x = mesh->mBitangents[i].x;
		vector.y = mesh->mBitangents[i].y;
		vector.z = mesh->mBitangents[i].z;
		vertex.Bitangent = vector;
		vertices.push_back(vertex);
	}

	//Experimental
	indexOffset = newmesh->m_vertexArray.size();
	newmesh->m_vertexArray.insert(newmesh->m_vertexArray.end(), vertices.begin(), vertices.end());


	// now wak through each of the mesh's faces
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		// retrieve all indices of the face and store them in the indices vector
		for (unsigned int j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}

    newmesh->m_indexArray.clear();
	for (unsigned int i = 0; i < indices.size(); i++)
	{
		newmesh->m_indexArray.push_back(indices[i] + indexOffset);
	}
}

AssetManager::~AssetManager()
{
	//for (auto& mesh : m_meshes) delete mesh;
}