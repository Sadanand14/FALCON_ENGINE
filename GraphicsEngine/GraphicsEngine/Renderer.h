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

public:
	Renderer();
	~Renderer();

	void Init();
	void SetDrawStates();
	void Draw();
};

#endif // !RENDERER_H

