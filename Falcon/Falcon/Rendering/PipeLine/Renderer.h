#ifndef RENDERER_H
#define RENDERER_H

#include <cstdlib>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <boost/container/set.hpp>
#include <boost/container/flat_map.hpp>
#include <Events/EventManager.h>

#include <stb_image.h>

#include <ThreadPool.h>

class RenderPass;
class Camera;
class Entity;
class RenderEvent;
class EventSystem;
class Renderable;

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

	virtual void SubscribeToEvents();

	virtual void ProcessEvents();

	void PrintReception();
};


////////////////////////////////////////////////////////////////////////////////
/**
 * Class Definition for the Rendering System Responsible for Rendering each frame.
 */
class Renderer
{
	RenderEventSystem* m_RES;
	glm::mat4 m_projection;
	boost::container::vector<RenderPass*, fmemory::StackSTLAllocator<RenderPass*>> m_renderPasses;
	boost::container::vector<Entity*, fmemory::StackSTLAllocator<Entity*>>* m_entities;

	//TODO: REMOVE
	Renderable* can;

public:
	Renderer();
	~Renderer();

	void Init();
	void CreateDrawStates();
	void SetDrawStates(boost::container::vector<Entity*, fmemory::StackSTLAllocator<Entity*>>* entities, glm::mat4 projection);
	void Update(Camera& cam,float deltaTime, boost::container::vector<Entity*, fmemory::StackSTLAllocator<Entity*>>* entities);
	void Draw(Camera &cam);
};

#endif // !RENDERER_H

