#ifndef RENDERER_H
#define RENDERER_H

#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "ShaderProgram.h"
#include "stb_image.h"
#include "RenderTexture.h"


class Renderer
{
private:
	VertexArray* va = nullptr;
	VertexBuffer* vb = nullptr;
	IndexBuffer* ib = nullptr;
	Shader* vs = nullptr, *fs = nullptr;
	ShaderProgram* sp = nullptr;
	Texture* texture = nullptr;

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

