#ifndef Asset_Manager_H
#define Asset_Manager_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <../Rendering/stb_image.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <../Rendering/Mesh.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>

class AssetManager 
{
private:

	// Process nodes
	static void ProcessNode(aiNode* node, const aiScene* scene, Mesh* mesh);
	static void ProcessMesh(aiMesh* mesh, const aiScene* scene, Mesh* newmesh);

	// Checks all material textures of a given type and loads the textures if they're not loaded yet.
	// The required info is returned as a Texture struct.

public:
	static Mesh* LoadModel(std::string const& path);


	AssetManager(std::string const& path, bool gamma = false);
	~AssetManager();
};


#endif 

