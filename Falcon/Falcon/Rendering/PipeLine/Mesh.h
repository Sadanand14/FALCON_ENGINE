#ifndef MESH_H
#define MESH_H

#include <framework.h>
#include <vector>
#include <string>
#include <boost/container/vector.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <ozz/base/containers/vector.h>
#include <ozz/base/maths/simd_math.h>
#include <ozz/base/maths/vec_float.h>

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
	ozz::Vector<ozz::math::Float4x4>::Std* m_animMats;
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
	void AddAnimationMatrices(ozz::Vector<ozz::math::Float4x4>::Std* animMats);
	void ClearWorldMatrices();
	u32 GetWorldMatrixAmount();
	void SetMaterial(Material* mat);
	Material* GetMaterial();
	void Bind();
};

#endif //!MESH_H