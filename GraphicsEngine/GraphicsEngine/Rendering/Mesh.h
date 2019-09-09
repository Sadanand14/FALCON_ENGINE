#ifndef MESH_H
#define MESH_H

#include <framework.h>
#include <vector>
#include <string>
#include <glm/gtc/matrix_transform.hpp>


#include "Shader.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexLayout.h"
#include "Texture.h"


/**
* Mesh Class tp store Mesh Data for Renderables.
*/
class Mesh {

private:
	//Render Data
	
	std::vector<Vertex> m_vertices;
	std::vector<unsigned int> m_indices;
	std::vector<Texture> m_textures;
	VertexArray  *m_VAO;
	VertexBuffer *m_VBO;
	IndexBuffer  *m_IBO;

	//Functions
	void SetupMesh();

public: 
	//Mesh Data
	

	//Constructor
	Mesh(const std::vector<Vertex>& vertices, 
		 const std::vector<unsigned int>& indices, 
		 const std::vector<Texture>& textures);

	
	~Mesh();

	//Functions	
	void DrawMesh(Shader shader);
	
};

#endif //!MESH_H