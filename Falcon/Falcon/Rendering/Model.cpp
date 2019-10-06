#include "Model.h"
#include "Log.h"


// Pass filepath to 3D model
Model::Model(std::string const& path, bool gamma /*false*/)
	: m_gammaCorrection(gamma)
{
	LoadModel(path);
	FL_ENGINE_INFO("INFO: Model loaded sussfully for {0}", path);
}

void Model::Draw(Shader shader)
{
	for (unsigned int i = 0; i < m_meshes.size(); i++)
		m_meshes[i]->DrawMesh(shader);
}

void Model::LoadModel(std::string const& path)
{
	// Read File (Assimp)
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
	// Error Check
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		FL_ENGINE_ERROR("ERROR::ASSIMP::{0}", importer.GetErrorString());
		return;
	}
	// Directory Path
	m_directory = path.substr(0, path.find_last_of('/'));

	// Process rootnode 
	ProcessNode(scene->mRootNode, scene);
}

void Model::ProcessNode(aiNode* node, const aiScene* scene)
{
	// Process each mesh located at the current node.
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		m_meshes.push_back(ProcessMesh(mesh, scene));
	}
	//Process children nodes.
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		ProcessNode(node->mChildren[i], scene);
	}
}

Mesh* Model::ProcessMesh(aiMesh* mesh, const aiScene* scene)
{
	// Data to load
	std::vector<Vertex, fmemory::STLAllocator<Vertex>> vertices;
	std::vector<unsigned int, fmemory::STLAllocator<unsigned int>> indices;
	std::vector<Texture, fmemory::STLAllocator<Texture>> textures;

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
	// now wak through each of the mesh's faces
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		// retrieve all indices of the face and store them in the indices vector
		for (unsigned int j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}
	
	// Process materials
	aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];


	//N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER. 
	// diffuse: texture_diffuseN
	// specular: texture_specularN
	// normal: texture_normalN

	// 1. Diffuse maps
	std::vector<Texture ,fmemory::STLAllocator<Texture>> diffuseMaps = LoadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
	textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
	// 2. Specular maps
	std::vector<Texture, fmemory::STLAllocator<Texture>> specularMaps = LoadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
	textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
	// 3. Normal maps
	std::vector<Texture, fmemory::STLAllocator<Texture>> normalMaps = LoadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
	textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
	// 4. Height maps
	std::vector<Texture, fmemory::STLAllocator<Texture>> heightMaps = LoadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
	textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

	// return a mesh object created from the extracted mesh data
	Mesh * tmpMesh = fmemory::fnew <Mesh>(vertices, indices, textures);
	return tmpMesh;
}

std::vector<Texture, fmemory::STLAllocator<Texture>> Model::LoadMaterialTextures(aiMaterial* mat, 
																				 aiTextureType type, std::string typeName)
{
	std::vector<Texture, fmemory::STLAllocator<Texture>> textures;
	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);
		// Check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
		bool skip = false;
		for (unsigned int j = 0; j < m_texturesLoaded.size(); j++)
		{
			if (std::strcmp(m_texturesLoaded[j].path.data(), str.C_Str()) == 0)
			{
				textures.push_back(m_texturesLoaded[j]);
				skip = true;
				break;
			}
		}
		if (!skip)
		{   // Load texture if not loaded
			Texture texture;
			texture.textureID = TextureFromFile(str.C_Str(), this->m_directory);
			texture.type = typeName;
			texture.path = str.C_Str();
			textures.push_back(texture);
			m_texturesLoaded.push_back(texture);
		}
	}
	return textures;
}

unsigned int TextureFromFile(const char* path, const std::string& directory, bool gamma)
{
	std::string filename(path);

	filename = directory + '/' + filename;

	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format = 0;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

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


Model::~Model()
{
	for (auto& mesh : m_meshes) fmemory::fdelete<Mesh>(mesh);
}