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

class Mesh {

private:

	//Experimental

	//Render Data
	VertexArray  *m_VAO = nullptr;
	VertexBuffer *m_VBO;
	IndexBuffer  *m_IBO;

	//Functions

public: 
	//Mesh Data
	std::vector<Vertex> m_vertexArray;
	std::vector<unsigned int> m_indexArray;

	Mesh();
	~Mesh();

	//Functions	
	void SetupMesh();
	void DrawMesh();
	
};

#endif //!MESH_H