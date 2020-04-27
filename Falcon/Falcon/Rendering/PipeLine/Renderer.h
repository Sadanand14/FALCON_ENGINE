#ifndef RENDERER_H
#define RENDERER_H

#include <cstdlib>
#include <CameraSystem.h>
#include "UIManager.h"
#include "Shader.h"
#include "System/Types.h"

#include <Events/RenderEvent.h>
#include <Events/EventManager.h>
#include <EntityInterface.h>
#include <AssetManager.h>


//#include <Camera.h>
#include <stb_image.h>

#include <ThreadPool.h>

#if defined(_DEBUG) || defined(DEBUG)
#include <Line.h>
#endif

#include <Waypoints.h>

class RenderPass;
class Camera;
class Entity;
struct RenderEvent;
class EventSystem;
class Renderable;
class Mesh;

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

//TODO: REMOVE
class Waypoint;

////////////////////////////////////////////////////////////////////////////////
/**
 * Class Definition for the Rendering System Responsible for Rendering each frame.
 */
class Renderer
{
	RenderEventSystem* m_RES = nullptr;
	UI::UI_Manager* m_UI = nullptr;
	GLFWwindow* m_window = nullptr ;
	glm::mat4 m_projection;
	//CameraSystem* m_camera = nullptr;
	boost::container::vector<RenderPass*> m_Game_renderPasses, m_Menu_renderPasses, m_Pause_renderPasses;
	boost::container::vector<Entity*>* m_entities;
	Mesh* m_terrainMesh = nullptr, * m_skyMesh = nullptr;
	static GLFWwindow* m_win;
	Texture m_brdfTextue;

	Waypoint wp;
#if defined(_DEBUG) || defined(DEBUG)
	Line wpTestLine;
#endif //Line debug

public:
	Renderer();
	~Renderer();

	inline UI::UI_Manager* GetUI() { return m_UI; }

	void Ingame_Update( float dt, boost::container::vector<Entity*>* entities);
	void Ingame_Draw();

	void Pause_Draw();
	void Pause_Update();

	void Menu_Update();
	void Menu_Draw();

	void Init(GLFWwindow* window);
	void CreateDrawStates(GLFWwindow* win);
	void SetDrawStates(boost::container::vector<Entity*>* entities, glm::mat4 projection);
};

#endif // !RENDERER_H

