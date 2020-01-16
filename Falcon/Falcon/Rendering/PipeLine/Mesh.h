#ifndef MESH_H
#define MESH_H

#include <framework.h>
#include <string>
#include <glm/gtc/matrix_transform.hpp>

#include "Renderable.h"


/**
* Mesh Class to store Mesh Data for Renderables.
*/
class Mesh : public Renderable
{
private:
	//Render Data
	VertexBuffer* m_VBO1;
	VertexBuffer* m_VBO2;
	IndexBuffer* m_IBO;
	boost::container::vector<glm::mat4, fmemory::STLAllocator<glm::mat4>> m_worldMats;

public:
	//Mesh Data
	u32* m_indexArray = nullptr;
	u32* m_indexOffsets = nullptr;
	u32 m_indexCount;
	u32 m_indexOffsetCount;

	Mesh();
	virtual ~Mesh();

	//Functions
	void Setup() override;
	void PreallocMatrixAmount(u32 maxMatrices);
	void AddWorldMatrix(const glm::mat4& mat);
	void ClearWorldMatrices();
	u32 GetWorldMatrixAmount();
	void Bind() override;
	//glm::vec3* GetVertexPositionsArray();
};

#endif //!MESH_H
