#ifndef RENDERER_H
#define RENDERER_H


#include <cstdlib>
#include "stb_image.h"

#include "Camera.h"
#include "Shader.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <boost/container/set.hpp>
#include "../Core/EntityInterface.h"
#include "../Core/AssetManager.h"

class Renderer
{
	Entity* entity;
	Shader* shader;
	boost::container::set<Mesh*> queuedMeshes;

public:
	Renderer();
	~Renderer();

	//Shader* m_shadyStuff = nullptr;
	//Model* m_nanosuit = nullptr;

	void Init();
	void CreateDrawStates();
	void SetDrawStates();
	void Update(int width, int height, float zoom, glm::mat4 view, float deltaTime);
	void Draw();

};

#endif // !RENDERER_H

