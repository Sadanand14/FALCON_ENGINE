#ifndef MESH_H
#define MESH_H

#include <framework.h>
#include <vector>
#include <string>
#include <glm/gtc/matrix_transform.hpp>


#include "Memory/fmemory.h"
#include "Shader.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexLayout.h"
#include "Texture.h"

class Mesh {

private:
	//Render Data
	
	std::vector<Vertex,fmemory::STLAllocator<Vertex>> m_vertices;
	std::vector<unsigned int, fmemory::STLAllocator<unsigned int>> m_indices;
	std::vector<Texture, fmemory::STLAllocator<Texture>> m_textures;
	VertexArray  *m_VAO;
	VertexBuffer *m_VBO;
	IndexBuffer  *m_IBO;

	//Functions
	void SetupMesh();

public: 
	//Mesh Data
	

	//Constructor
	Mesh(const std::vector<Vertex,fmemory::STLAllocator<Vertex>>& vertices, 
		 const std::vector<unsigned int, fmemory::STLAllocator<unsigned int>>& indices,
		 const std::vector<Texture, fmemory::STLAllocator<Texture>>& textures);

	
	~Mesh();

	//Functions	
	void DrawMesh(Shader shader);
	
};

#endif //!MESH_H