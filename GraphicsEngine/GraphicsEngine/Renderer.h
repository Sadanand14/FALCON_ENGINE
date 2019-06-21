#ifndef RENDERER_H
#define RENDERER_H

#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "ShaderProgram.h"
#include <glad/glad.h>


class Renderer
{
private:
	VertexArray* va;
	VertexBuffer* vb;
	IndexBuffer* ib;
	Shader* vs, *fs;
	ShaderProgram* sp;

public:
	Renderer();
	~Renderer();

	void Init();
	void CreateDrawStates();
	void SetDrawStates();
	void Update(float);
	void Draw(float);
};

#endif // !RENDERER_H

