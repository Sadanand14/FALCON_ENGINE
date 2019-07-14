#ifndef MESH_H
#define MESH_H

#include <framework.h>
#include <vector>
#include <string>
#include <glm/gtc/matrix_transform.hpp>
#include "Shader.h"
#include "VertexLayout.h"

using namespace std;

struct Texture {
	unsigned int textureID = 0;
	string type;
	string path;
	
};

class Mesh {

private:
	//Render Data
	unsigned int VBO, EBO;

	//Functions
	void SetupMesh();

public: 
	//Mesh Data
	vector<Vertex> m_vertices;
	vector<unsigned int> m_indices;
	vector<Texture> m_textures;
	unsigned int VAO;

	//Constructor
	Mesh(const vector<Vertex>& vertices, const vector<unsigned int>& indices, const vector<Texture>& textures);

	//Functions	
	void DrawMesh(Shader shader);
};

#endif