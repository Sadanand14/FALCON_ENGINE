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
#include "RenderPass.h"
#include "MeshRenderPass.h"
#include "ParticleRenderPass.h"

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

	static void ShutDown();

	//static void ShutDown() 
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
	Shader* shader;
	Shader* particleShader;
	RenderEventSystem* m_RES;
	boost::container::vector<RenderPass*, fmemory::StackSTLAllocator<RenderPass*>> m_renderPasses;

public:
	Renderer();
	~Renderer();

	//Shader* m_shadyStuff = nullptr;
	//Model* m_nanosuit = nullptr;

	void Init();
	void CreateDrawStates();
	void SetDrawStates(boost::container::vector<Entity*, fmemory::StackSTLAllocator<Entity*>>* entities);
	void Update(int width, int height, Camera &cam, float deltaTime, boost::container::vector<Entity*, fmemory::StackSTLAllocator<Entity*>>* entities);
	void Draw(boost::container::vector<Entity*, fmemory::StackSTLAllocator<Entity*>>* entities);
};

#endif // !RENDERER_H

