#ifndef MODEL_H
#define MODEL_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <../Rendering/stb_image.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <../Rendering/Mesh.h>
#include "Shader.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>

class Model
{
private:
	//Functions
	// Load model (assimp supported extension) and store mesh data in meshes
	void LoadModel(string const& path);

	// Process nodes
	void ProcessNode(aiNode* node, const aiScene* scene);

	Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);

	// Checks all material textures of a given type and loads the textures if they're not loaded yet.
	// The required info is returned as a Texture struct.
	vector<Texture> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName);

public:
	/*  Model Data */
	vector<Texture> m_texturesLoaded;
	vector<Mesh> m_meshes;
	string m_directory;
	bool m_gammaCorrection;

	//Functions
	// Pass filepath to 3D model
	Model(string const& path, bool gamma = false) : m_gammaCorrection(gamma)
	{
		LoadModel(path);
	}

	// Draw Model
	void Draw(Shader shader);


};

//Load Texture from file
unsigned int TextureFromFile(const char* path, const string& directory, bool gamma);
unsigned int TextureFromFile(const char* path, const string& directory, bool gamma = false);


#endif