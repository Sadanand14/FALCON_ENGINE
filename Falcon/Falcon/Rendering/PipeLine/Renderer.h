#ifndef RENDERER_H
#define RENDERER_H

#include <cstdlib>

#include "Shader.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <boost/container/set.hpp>

#include <Events/RenderEvent.h>
#include <Events/EventManager.h>
#include <EntityInterface.h>
#include <AssetManager.h>

#include <Camera.h>
#include <stb_image.h>

#include <ThreadPool.h>

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

	void PrintReception();
};


////////////////////////////////////////////////////////////////////////////////
/**
*Class Definition for the Rendering System Responsible for Rendering each frame.
*/
class Renderer
{
	boost::container::vector<Entity*, fmemory::STLAllocator<Entity*>> m_entity;
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

	inline void SetEntities(boost::container::vector<Entity*, fmemory::STLAllocator<Entity*>> entities) 
	{
		m_entity = entities;
	}
};

#endif // !RENDERER_H

