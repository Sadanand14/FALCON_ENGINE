#ifndef RENDERABLE_H
#define RENDERABLE_H

#include "System/Types.h"

#include "Shader.h"
#include "Material.h"

#include <BufferDefinitions/VertexArray.h>
#include <BufferDefinitions/VertexBuffer.h>
#include <BufferDefinitions/IndexBuffer.h>
#include <BufferDefinitions/VertexLayout.h>
#include <Types.h>

class Renderable
{
protected:
	Material* m_material = nullptr;
	VertexArray* m_VAO = nullptr;

public:
	Renderable();
	virtual ~Renderable();
	boost::container::vector<Vertex, fmemory::STLAllocator<Vertex>> m_vertexArray;
	virtual void Setup();
	virtual void Bind();
	inline void SetMaterial(Material* mat) { m_material = mat; }
	inline Material* GetMaterial() const { return m_material; }
};

#endif