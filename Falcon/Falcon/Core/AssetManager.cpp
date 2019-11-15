#include "AssetManager.h"
#include <Log.h>


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

	//Experimental
	Mesh* newmesh = fmemory::fnew<Mesh>();
	newmesh->m_vertexArray.clear();
	newmesh->m_indexArray.clear();
	////////////////


	// Process rootnode
	boost::container::map<std::string, u32> boneName;
	boost::container::map<u32, Bone> vertBones;
	u32 boneCount = 0;
	ProcessNode(scene->mRootNode, scene, newmesh, boneName, vertBones, boneCount);

	for(auto it = vertBones.begin(); it != vertBones.end(); it++)
	{
		printf("%d %d %d %d\n", it->second.ids);
		printf("%f %f %f %f\n", it->second.weights);
		newmesh->m_vertexArray[it->first].bone = it->second;
		//printf("%d %d %d %d\n", newmesh->m_vertexArray[it->first].bone.ids);
		//printf("%f %f %f %f\n", newmesh->m_vertexArray[it->first].bone.weights);
	}

	FL_ENGINE_INFO("Vertices :{0}", newmesh->m_vertexArray.size());
	FL_ENGINE_INFO("Indices :{0}", newmesh->m_indexArray.size());
	newmesh->SetupMesh();
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
		switch(nrComponents) {
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
	//TODO: Change this to actually load a material using json and remove tmp things
	Material* mat = fmemory::fnew<Material>();
	mat->albedoTex.textureID = LoadTexture("../Assets/Models/cerb/cerberus_albedo.tga");
	//mat->roughnessTex.textureID = LoadTexture("../Assets/Models/cerb/cerberus_rough.tga");
	//mat->normalTex.textureID = LoadTexture("../Assets/Models/cerb/cerberus_normal.tga");
	//mat->metallicTex.textureID = LoadTexture("../Assets/Models/cerb/cerberus_metal.tga");
	//mat->aoTex.textureID = LoadTexture("../Assets/Models/cerb/cerberus_ao.tga");

	return mat;
}

Animation* AssetManager::LoadAnimation(std::string const &path)
{
	//Load the skeleton
	ozz::io::File skeleFile("../Assets/Models/def/astro_max_skeleton.ozz", "rb");

	//Check if open
	if (!skeleFile.opened()) {
		ozz::log::Err() << "Cannot open file " << path << "." << std::endl;
		return nullptr;
	}

	ozz::io::IArchive skeleArchive(&skeleFile);

	if (!skeleArchive.TestTag<ozz::animation::Skeleton>()) {
		ozz::log::Err() << "Archive doesn't contain the expected object type." << std::endl;
		return nullptr;
	}

	//Load the animation
	ozz::io::File animFile("../Assets/Models/def/astro_max_animation.ozz", "rb");

	//Check if open
	if (!animFile.opened()) {
		ozz::log::Err() << "Cannot open file " << path << "." << std::endl;
		return nullptr;
	}

	ozz::io::IArchive animArchive(&animFile);

	if (!animArchive.TestTag<ozz::animation::Animation>()) {
		ozz::log::Err() << "Archive doesn't contain the expected object type." << std::endl;
		return nullptr;
	}

	Animation* anim = fmemory::fnew<Animation>();
	skeleArchive >> anim->m_skel;
	animArchive >> anim->m_anim;

	anim->update();

	return anim;
}


//Private Functions
/**
*This function iterates through the scene node provided by Assimp and Retrives all the mesh data.
*
*@param[in] An aiNode* type pointer(defined in assimp Library)
*@param[in] An aiScene* type pointer(defined in assimp Library)
*@param[in] A new Mesh pointer to store all the mesh data into.
*/
void AssetManager::ProcessNode(aiNode* node, const aiScene* scene, Mesh* newmesh, boost::container::map<std::string, u32> &bones, boost::container::map<u32, Bone> &vertBones, u32 &boneCount)
{
	// Process each mesh located at the current node.
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		ProcessMesh(mesh, newmesh, bones, vertBones, boneCount);
		//m_meshes.push_back(ProcessMesh(mesh, scene, newmesh));
	}
	//Process children nodes.
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		ProcessNode(node->mChildren[i], scene, newmesh, bones, vertBones, boneCount);
	}
}

/**
*This function retrieves the mesh data and stores in the provided mesh location.
*
*@param[in] An aiNode* type pointer(defined in assimp Library)
*@param[in] A new Mesh pointer to store all the mesh data into.
*/
void AssetManager::ProcessMesh(aiMesh* mesh, Mesh* newmesh, boost::container::map<std::string, u32> &bones, boost::container::map<u32, Bone> &vertBones, u32 &boneCount)
{
	// Data to load
	size_t indexOffset = 0;
	std::vector<Vertex, fmemory::STLAllocator<Vertex>> vertices;
	std::vector<u32, fmemory::STLAllocator<unsigned int>> indices;
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
			//TODO: Add support for models that use more than one set of texture coordinates.
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

	ProcessBones(mesh, newmesh, bones, vertBones, boneCount, newmesh->m_vertexArray.size());
	newmesh->m_vertexArray.insert(newmesh->m_vertexArray.end(), vertices.begin(), vertices.end());

	newmesh->m_indexOffsets.push_back(newmesh->m_indexArray.size());

	// now wak through each of the mesh's faces
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		// retrieve all indices of the face and store them in the indices vector
		for (unsigned int j = 0; j < face.mNumIndices; j++)
			newmesh->m_indexArray.push_back(face.mIndices[j]);
	}

}

void AssetManager::ProcessBones(aiMesh* mesh, Mesh* newmesh, boost::container::map<std::string, u32> &bones, boost::container::map<u32, Bone> &vertBones, u32 &boneCount, u32 vertexOffset)
{
	for(u32 i = 0; i < mesh->mNumBones; i++)
	{
		u32 boneInd = 0;
		std::string boneName = mesh->mBones[i]->mName.data;

		if(bones.find(boneName) == bones.end())
		{
			boneInd = boneCount;
			boneCount++;
		}

		else
			boneInd = bones[boneName];

		bones[boneName] = boneInd;

		for(u32 j = 0; j < mesh->mBones[i]->mNumWeights; j++)
		{
			u32 vertID = vertexOffset + mesh->mBones[i]->mWeights[j].mVertexId;
			float weight = mesh->mBones[i]->mWeights[j].mWeight;

			//auto vert = ;
			if(vertBones.find(vertID) != vertBones.end())
			{
				for(u32 k = 1; k < 4; k++)
				{
					if(vertBones[vertID].weights[k] == 0.0f)
					{
						vertBones[vertID].ids[k] = boneInd;
						vertBones[vertID].weights[k] = weight;
						break;
					}
				}
			}
			else
				vertBones[vertID] = Bone { glm::uvec4(boneInd, 0, 0, 0), glm::vec4(weight, 0.0f, 0.0f, 0.0f) };
		}
	}
}