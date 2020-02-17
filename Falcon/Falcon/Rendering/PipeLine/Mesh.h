#ifndef MESH_H
#define MESH_H

#include <framework.h>
#include <string>
#include <System/Types.h>

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
	bool m_transparent = false;

public:
	//Mesh Data
	boost::container::vector<u32, fmemory::STLAllocator<u32>> m_indexArray;
	boost::container::vector<u32, fmemory::STLAllocator<u32>> m_indexOffsets;

	Mesh();
	virtual ~Mesh();

	//Functions
	void Setup() override;
	void PreallocMatrixAmount(u32 maxMatrices);
	void AddWorldMatrix(const glm::mat4& mat);
	void ClearWorldMatrices();
	u32 GetWorldMatrixAmount();
	void Bind() override;
	void GetVertexPositionsArray(std::vector < glm::vec3, fmemory::STLAllocator<glm::vec3>>&);

	inline VertexBuffer* GetVB() { return m_VBO1; }
	inline VertexBuffer* GetVertexBuffer() { return m_VBO1; }
	inline void SetTransparent(bool transparent) { m_transparent = transparent; }
	inline bool GetTransparent() { return m_transparent; }
};

#endif //!MESH_H
