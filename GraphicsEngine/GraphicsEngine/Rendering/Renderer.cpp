#include "Renderer.h"

Renderer::Renderer()
{
	Init();
}

Renderer::~Renderer()
{
	delete va;
	delete vb;
	delete ib;
	delete vs;
	delete fs;
	delete sp;
	delete texture;
}

//renderer initialization
void Renderer::Init()
{

}

void Renderer::CreateDrawStates() 
{
	float vertices[] =
	{
		-0.5f, -0.5f, 0.0f, 0.0f,
		0.5f, -0.5f, 1.0f, 0.0f,
		0.5f, 0.5f, 1.0f, 1.0f,
		-0.5f, 0.5f, 0.0f, 1.0f
	};

	unsigned int indices[] =
	{
		0,1,2,
		2,3,0
	};

	va = new VertexArray();             //Create vertex Array
	vb = new VertexBuffer(vertices, sizeof(float) * 16); // Create VertexBuffer
	
	VertexLayout vl; //Create Vertex Layout
	vl.Push < float>(2); //Push in an attribute layout
	vl.Push < float>(2); //Push in another attribute layout
	va->AddBuffer(vb, vl); // add buffer as well as its attribute layout
	ib = new IndexBuffer(indices, 6); // generate an index buffer
	vs = new Shader("Shader/VertexShader.vert"); // generate a vertex shader
	fs = new Shader("Shader/FragmentShader.frag"); // generate a fragment shader
	sp = new ShaderProgram(fs->GetID(), vs->GetID()); // generate a shader program to make use of those shaders
	texture = new Texture("../Assets/Textures/water.jpg"); // load texture
}

//function where we set all the states for the draw function
void Renderer::SetDrawStates()
{
	va->Bind();// bind the vertex array object to the context (auto binds the buffer bound to it as well)
	ib->Bind();// bind the index buffer to the context
	sp->Use(); // bind the shader program to the context 

	texture->Bind();// Bind the texture to slot 0;
	sp->SetUniform1i("u_color", 0);// Tell the texture sampler to pick up the texture from slot 0;
}


// update function to update the transformations of our objects
void Renderer::Update(float dt) 
{
	
}

//main draw function
void Renderer::Draw(float dt)
{
	glClearColor(0.0f, 0.5f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}
