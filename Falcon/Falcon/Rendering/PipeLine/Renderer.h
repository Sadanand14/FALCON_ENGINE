#ifndef RENDERER_H
#define RENDERER_H

#include <cstdlib>

#include "Shader.h"
#include "System/Types.h"

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
	GLFWwindow* m_window;
	glm::mat4 m_projection;
	boost::container::vector<RenderPass*, fmemory::STLAllocator<RenderPass*>> m_Game_renderPasses, m_Menu_renderPasses, m_Pause_renderPasses;
	boost::container::vector<Entity*, fmemory::STLAllocator<Entity*>>* m_entities;
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


	void Ingame_Update(Camera& cam, float dt, boost::container::vector<Entity*, fmemory::STLAllocator<Entity*>>* entities);
	void Ingame_Draw(Camera& cam);

	void Pause_Draw();
	void Pause_Update();

	void Menu_Update();
	void Menu_Draw();

	void Init(GLFWwindow* window);
	void CreateDrawStates();
	void SetDrawStates(boost::container::vector<Entity*, fmemory::STLAllocator<Entity*>>* entities, glm::mat4 projection);

	//Exposed for use in game
	static void uiPrev3();
};

#endif // !RENDERER_H

