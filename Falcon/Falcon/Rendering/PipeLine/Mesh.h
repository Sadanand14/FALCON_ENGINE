#ifndef MESH_H
#define MESH_H

#include <framework.h>
#include <vector>
#include <string>
#include <boost/container/vector.hpp>
#include <glm/gtc/matrix_transform.hpp>


#include "Memory/fmemory.h"
#include "Shader.h"
#include "Material.h"

#include <BufferDefinitions/VertexArray.h>
#include <BufferDefinitions/VertexBuffer.h>
#include <BufferDefinitions/IndexBuffer.h>
#include <BufferDefinitions/VertexLayout.h>

#include <Types.h>


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
	boost::container::vector<glm::mat4> m_animationMats;

	//Functions

public:
	//Mesh Data
	boost::container::vector<Vertex> m_vertexArray;
	boost::container::vector<u32> m_indexArray;
	boost::container::vector<u32> m_indexOffsets;
	boost::container::vector<glm::mat4> m_transforms;

	Mesh();
	~Mesh();

	//Functions
	void SetupMesh();
	void PreallocMatrixAmount(u32 maxMatrices);
	void AddWorldMatrix(const glm::mat4 &mat);
	void AddAnimationMatrix(const glm::mat4 &mat);
	void ClearWorldMatrices();
	void ClearAnimationMatrices();
	u32 GetWorldMatrixAmount();
	u32 GetAnimationMatrixAmount();
	void SetMaterial(Material* mat);
	Material* GetMaterial();
	void Bind();

};

#endif //!MESH_H