#ifndef MESH_H
#define MESH_H

#include <framework.h>
#include <vector>
#include <string>
#include <boost/container/vector.hpp>
#include <glm/gtc/matrix_transform.hpp>


#include "Memory/fmemory.h"
#include "Shader.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexLayout.h"
#include "Material.h"
#include "../System/Types.h"


/**
* Mesh Class tp store Mesh Data for Renderables.
*/
class Mesh {

private:

	//Experimental

	//Render Data
	VertexArray* m_VAO = nullptr;
	VertexBuffer* m_VBO1;
	VertexBuffer* m_VBO2;
	IndexBuffer* m_IBO;
	Material* m_material = nullptr;
	boost::container::vector<glm::mat4> m_worldMats;
	
	//Functions

public:
	//Mesh Data
	boost::container::vector<Vertex> m_vertexArray;
	boost::container::vector<u32> m_indexArray;
	boost::container::vector<u32> m_indexOffsets;

	Mesh();
	~Mesh();

	//Functions
	void SetupMesh();
	void PreallocMatrixAmount(u32 maxMatrices);
	void AddWorldMatrix(const glm::mat4 &mat);
	void ClearWorldMatrices();
	u32 GetWorldMatrixAmount();
	void SetMaterial(Material* mat);
	Material* GetMaterial();
	void Bind();

};

#endif //!MESH_H