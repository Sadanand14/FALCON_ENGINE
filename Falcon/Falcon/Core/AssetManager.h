#ifndef Asset_Manager_H
#define Asset_Manager_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <../Rendering/stb_image.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <../Rendering/Mesh.h>
#include "../Rendering/Shader.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>

class AssetManager 
{
private:
	unsigned int indexOffset;
	//Functions
// Load model (assimp supported extension) and store mesh data in meshes
	void LoadModel(std::string const& path);

	// Process nodes
	void ProcessNode(aiNode* node, const aiScene* scene, Mesh* mesh);

	Mesh* ProcessMesh(aiMesh* mesh, const aiScene* scene, Mesh* newmesh);

	// Checks all material textures of a given type and loads the textures if they're not loaded yet.
	// The required info is returned as a Texture struct.
	std::vector<Texture> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);

public:
	/*  Model Data */
	std::vector<Texture> m_texturesLoaded;
	std::vector<Mesh*> m_meshes;
	std::string m_directory;
	bool m_gammaCorrection;


	AssetManager(std::string const& path, bool gamma = false);
	~AssetManager();
	// Draw Model
	void Draw(Shader shader);
};

unsigned int TextureFromFile(const char* path, const std::string& directory, bool gamma);
unsigned int TextureFromFile(const char* path, const std::string& directory, bool gamma = false);

#endif 

