#ifndef RENDERER_H
#define RENDERER_H


#include "../System/Camera.h"
#include "..//Core/RenderEvent.h"
#include "..//Core/EventManager.h"
#include "Shader.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "../Core/EntityInterface.h"
#include "../Core/AssetManager.h"
#include "../System/ThreadPool.h"


class RenderEventSystem : public EventSystem
{
private:
	static RenderEventSystem* m_instance;
	RenderEventSystem();

public:
	static RenderEventSystem* GetInstance()
	{
		if (m_instance == nullptr)
		{
			m_instance = new RenderEventSystem();
		}
		return m_instance;
	}

	virtual void SubscribeToEvents();

	virtual void ProcessEvents();
};


////////////////////////////////////////////////////////////////////////////////

class Renderer
{
	Entity* entity;
	Shader* shader;
	RenderEventSystem* m_RES;

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

