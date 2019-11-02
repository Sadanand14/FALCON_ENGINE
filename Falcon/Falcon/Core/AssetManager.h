#ifndef Asset_Manager_H
#define Asset_Manager_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stb_image.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <PipeLine/Mesh.h>
#include <PipeLine/Material.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>

/**
* A class for defining procedures of extraction of asset data from external files and storing them in the engine.
* Most Methods are primarily static and this class stores no data.
*/
class AssetManager
{
private:

	// Process nodes
	static void ProcessNode(aiNode* node, const aiScene* scene, Mesh* mesh);
	static void ProcessMesh(aiMesh* mesh, Mesh* newmesh);

	// Checks all material textures of a given type and loads the textures if they're not loaded yet.
	// The required info is returned as a Texture struct.

public:
	static Mesh* LoadModel(std::string const& path);
	static u32 LoadTexture(std::string const& path);
	static Material* LoadMaterial(std::string const& path);

};


#endif

