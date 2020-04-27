#ifndef MESH_H
#define MESH_H

#include <framework.h>
#include <string>
#include <System/Types.h>

#include <RenderPasses/Renderable.h>
/**
* Mesh Class to store Mesh Data for Renderables.
*/
class Mesh : public Renderable
{
private:
	//Render Data
	VertexBuffer* m_VBO1;
	VertexBuffer* m_VBO2;
	VertexBuffer* m_VBO3;
	IndexBuffer* m_IBO;
	boost::container::vector<glm::mat4> m_worldMats;
	boost::container::vector<glm::mat3> m_normalMats;
	bool m_transparent = false;

public:
	//Mesh Data
	boost::container::vector<u32> m_indexArray;
	boost::container::vector<u32> m_indexOffsets;

	Mesh();
	virtual ~Mesh();

	//Functions
	void Setup() override;
	void PreallocMatrixAmount(u32 maxMatrices);
	void AddWorldAndNormalMatrix(const glm::mat4& mat, const glm::mat3& normal);
	void ClearWorldMatrices();
	u32 GetWorldMatrixAmount();
	void Bind() override;
	void GetVertexPositionsArray (std::vector < glm::vec3>&) const;

	inline VertexBuffer* GetVB() { return m_VBO1; }
	inline VertexBuffer* GetVertexBuffer() { return m_VBO1; }
	inline void SetTransparent(bool transparent) { m_transparent = transparent; }
	inline bool GetTransparent() const{ return m_transparent; }
};

#endif //!MESH_H
