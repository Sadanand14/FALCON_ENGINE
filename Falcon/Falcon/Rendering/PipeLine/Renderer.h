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
struct RenderEvent;
class EventSystem;
class Renderable;
class Mesh;

//TODO: REMOVE THIS
class Label;
class Button;
class Image;
class Panel;
class Slider;

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
	static GLFWwindow* m_win;

	//TODO: REMOVE
	Renderable* can;
	static Button* prev;
	static Button* next;
	static Image* bg;
	static Slider* wingAngle;
	static Slider* gearRatio;
	static Slider* suspension;

	static Texture uiPage1;
	static Texture uiPage2;
	static Texture uiPage3;

	static void uiNext0();
	static void uiNext1();
	static void uiNext2();

	static void uiPrev1();
	static void uiPrev2();

public:
	Renderer();
	~Renderer();

	void Init();
	void CreateDrawStates(GLFWwindow* win);
	void SetDrawStates(boost::container::vector<Entity*, fmemory::StackSTLAllocator<Entity*>>* entities, glm::mat4 projection);
	void Update(Camera& cam,float deltaTime, boost::container::vector<Entity*, fmemory::StackSTLAllocator<Entity*>>* entities);
	void Draw(Camera &cam);

	//Exposed for use in game
	static void uiPrev3();
};

#endif // !RENDERER_H

