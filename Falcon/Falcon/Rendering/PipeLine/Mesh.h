#ifndef MESH_H
#define MESH_H

#include <framework.h>
#include <vector>
#include <string>
#include <boost/container/vector.hpp>
#include <glm/gtc/matrix_transform.hpp>


#include <Memory/fmemory.h>
#include "Shader.h"
#include "Material.h"

#include <BufferDefinitions/VertexArray.h>
#include <BufferDefinitions/VertexBuffer.h>
#include <BufferDefinitions/IndexBuffer.h>
#include <BufferDefinitions/VertexLayout.h>

#include <Types.h>

typedef boost::container::vector<glm::vec3, fmemory::StackSTLAllocator<glm::vec3>> boundingVector;

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
	std::string m_path;
	std::string m_jsonPath;

	//bounding Box stuff
	boundingVector m_boundingCorners;

	//Functions

public:

	//bounding Box stuff
	inline boundingVector GetMesh() { return m_boundingCorners; }
	inline void SetBoundingVector(boundingVector value) { m_boundingCorners = value; }

	//Mesh Data
	boost::container::vector<Vertex> m_vertexArray;
	boost::container::vector<u32> m_indexArray;
	boost::container::vector<u32> m_indexOffsets;

	Mesh();
	~Mesh();

	//Functions
	void SetupMesh();
	void PreallocMatrixAmount(u32 maxMatrices);
	void AddWorldMatrix(const glm::mat4& mat);
	void ClearWorldMatrices();
	u32 GetWorldMatrixAmount();
	void SetMaterial(Material* mat);
	Material* GetMaterial();
	void Bind();

	inline const std::string& GetJsonPath() const { return m_jsonPath; }
	inline void SetJsonPath(const std::string& jsonPath) { m_jsonPath = jsonPath; }
	inline const std::string& GetPath() const { return m_path; }
	inline void SetPath(const std::string& path) { m_path = path; }

};

#endif //!MESH_H