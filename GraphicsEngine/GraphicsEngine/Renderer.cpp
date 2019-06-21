#include "Renderer.h"

Renderer::Renderer()
{
	Init();
}

Renderer::~Renderer()
{

}

//renderer initialization
void Renderer::Init()
{

}

//function where we set all the states for the draw function
void Renderer::SetDrawStates()
{
	float vertices[] =
	{
		-0.5f, -0.5f,
		0.5f, -0.5f,
		0.5f, 0.5f,
		-0.5f, 0.5f
	};

	unsigned int indices[] =
	{
		0,1,2,
		2,3,0
	};

	VertexArray* va = new VertexArray();
	VertexBuffer* vb = new VertexBuffer(vertices, sizeof(float) * 8);

	VertexLayout vl;
	vl.Push < float>(2);
	va->AddBuffer(*vb, vl);

	IndexBuffer* ib = new IndexBuffer(indices, 6);

	Shader* vs = new Shader("Shader/VertexShader.shader");
	Shader* fs = new Shader("Shader/FragmentShader.shader");

	ShaderProgram* sp = new ShaderProgram(fs->GetID(), vs->GetID());

	sp->Use();
}

//main draw function
void Renderer::Draw(float dt)
{
	glClearColor(0.0f, 0.5f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

}
