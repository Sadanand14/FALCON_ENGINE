#ifndef MESH_H
#define MESH_H

#include <framework.h>
#include <vector>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Shader.h"

using namespace std;

struct Vertex {
	//Position 
	glm::vec3 Position;
	//Normal
	glm::vec3 Normal;
	//Texture Coordinates
	glm::vec2 TexCoords;
	//Tangent
	glm::vec3 Tangent;
	//Bitangent
	glm::vec3 Bitangent;
};

struct Texture {
	unsigned int textureID = 0;
	string type;
	string path;
	
};

class Mesh {

public: 
	//Mesh Data
	vector<Vertex> m_vertices;
	vector<unsigned int> m_indices;
	vector<Texture> m_textures;
	unsigned int VAO;

	//Functions
	Mesh(const vector<Vertex>& vertices, const vector<unsigned int>& indices, const vector<Texture>& textures)
		: m_vertices(vertices)
		, m_indices(indices)
		, m_textures(textures)
	{
		setupMesh();
	}

	void drawMesh(Shader shader);

private:
	//Render Data
	unsigned int VBO, EBO;

	//Functions
	void setupMesh();
};

#endif