#ifndef game_h
#define game_h

#include "InputReceiver.h"
#include "Log.h"
#include <framework.h>
#include <Events/RenderEvent.h>
#include <Events/EventManager.h>
#include <Memory/fmemory.h>
#include <PipeLine/Renderer.h>
#include <Camera.h>
#include <Scene/SceneGraph.h>
#include <PipeLine/Octree.h>
#include "ParticleSystem.h"
#include <Audio/AudioEngine.h>


namespace gameLoop
{
	/**
	* Main Game Class Responsible for owning and running all subsystems
	*/
	class Game {

		Timer* m_timer;
		Renderer* m_renderer;
		WindowClass* m_window;
		InputReceiver* m_input;
		Scene::SceneGraph* m_scene;
		Rendering::Octree* m_octree;
		ParticleSystem* m_particleSystem = nullptr;
		//Scene* m_scene;
	public:
		bool m_gameCrashed, m_windowClosed;
		//Audio Engine
		CAudioEngine m_audio;

		Game();
		~Game();
		void ProcessInputs(float dt);
		bool Initialize();
		void Update();
	};

}
#endif
