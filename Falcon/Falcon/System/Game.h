#ifndef game_h
#define game_h

#include "InputReceiver.h"
#include "Log.h"
#include <framework.h>
#include <thread/thread.hpp>
#include <Events/RenderEvent.h>
#include <Events/EventManager.h>
#include <Memory/fmemory.h>
#include <PipeLine/Renderer.h>
#include <Camera.h>
#include <Scene/SceneGraph.h>
#include <PipeLine/Octree.h>

namespace gameLoop
{
	class Game {

		Timer* m_timer;
		Renderer* m_renderer;
		WindowClass* m_window1;
		InputReceiver* m_inputClass;
		Scene::SceneGraph* m_scene;
		Rendering::Octree* m_octree;
		//Scene* m_scene;
	public:
		bool m_gameCrashed, m_windowClosed;

		Game();
		~Game();

		bool Initialize();
		void Update();
	};

	void mouse_callback(GLFWwindow* window, double xpos, double ypos);
	void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

#endif
}