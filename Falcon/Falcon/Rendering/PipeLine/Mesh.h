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
	boost::container::vector<glm::mat4> m_worldMats;
	std::string m_path;
	std::string m_jsonPath;

	//Functions

public:
	//Mesh Data
	boost::container::vector<u32> m_indexArray;
	boost::container::vector<u32> m_indexOffsets;

	Mesh();
	~Mesh();

	//Functions
	void Setup() override;
	void PreallocMatrixAmount(u32 maxMatrices);
	void AddWorldMatrix(const glm::mat4& mat);
	void ClearWorldMatrices();
	u32 GetWorldMatrixAmount();
	void Bind() override;
	glm::vec3* GetVertexPositionsArray();


	inline const std::string& GetJsonPath() const { return m_jsonPath; }
	inline void SetJsonPath(const std::string& jsonPath) { m_jsonPath = jsonPath; }
	inline const std::string& GetPath() const { return m_path; }
	inline void SetPath(const std::string& path) { m_path = path; }

};

#endif //!MESH_H