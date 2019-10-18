#ifndef RENDERER_H
#define RENDERER_H


#include <Core/RenderEvent.h>
#include <Core/EventManager.h>
#include <cstdlib>
#include "stb_image.h"
#include "Camera.h"
#include "Shader.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <boost/container/set.hpp>
#include <Core/EntityInterface.h>
#include <Core/AssetManager.h>
#include <System/ThreadPool.h>

void PrintReception();

/**
* Class Definition for a Render Event System which will respond to all Render Type Events.
*/
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
			//m_instance = fmemory::fnew<RenderEventSystem>();
		}
		return m_instance;
	}

	virtual void SubscribeToEvents();

	virtual void ProcessEvents();
};


////////////////////////////////////////////////////////////////////////////////
/**
*Class Definition for the Rendering System Responsible for Rendering each frame.
*/
class Renderer
{
	Entity* entity;
	Shader* shader;
	RenderEventSystem* m_RES;
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

