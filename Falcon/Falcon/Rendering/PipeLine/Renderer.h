#ifndef RENDERER_H
#define RENDERER_H

#include <cstdlib>

#include "Shader.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <boost/container/set.hpp>
#include <boost/container/flat_map.hpp>

#include <Events/RenderEvent.h>
#include <Events/EventManager.h>
#include <EntityInterface.h>
#include <AssetManager.h>

#include <Camera.h>
#include <stb_image.h>

#include <ThreadPool.h>

class RenderPass;
class Camera;
class Entity;
class RenderEvent;
class EventSystem;
class Renderable;
class Mesh;

//TODO: REMOVE THIS
class Label;
class Button;

void PrintReception();

/**
* Class Definition for a Render Event System which will respond to all Render Type Events.
*/
class RenderEventSystem : public EventSystem
{
private:
	friend class Renderer;
	static RenderEventSystem* m_instance;
	Mesh* m_Skymesh;
	Mesh* m_TerrainMesh;
	RenderEventSystem();
	Mesh* m_terrainMesh, * m_skyMesh;

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
	inline Mesh* GetSkyMesh()const { return m_skyMesh; }
	inline Mesh* GetTerrainMesh() const { return m_terrainMesh; }
	static void ShutDown();

	virtual void SubscribeToEvents();

	virtual void ProcessEvents();

	void PrintReception();
};

class GLFWwindow;

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
	Mesh* m_terrainMesh = nullptr, * m_skyMesh = nullptr;
	GLFWwindow* m_win = nullptr;

	//TODO: REMOVE
	Renderable* can;
	Label* l;
	Button* b;
public:
	Renderer();
	~Renderer();

	void Init();
	void CreateDrawStates(GLFWwindow* win);
	void SetDrawStates(boost::container::vector<Entity*, fmemory::StackSTLAllocator<Entity*>>* entities, glm::mat4 projection);
	void Update(Camera& cam,float deltaTime, boost::container::vector<Entity*, fmemory::StackSTLAllocator<Entity*>>* entities);
	void Draw(Camera &cam);
};

#endif // !RENDERER_H

