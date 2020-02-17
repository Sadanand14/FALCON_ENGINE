#ifndef ENGINE_H
#define ENGINE_H

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
	class Engine {

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

		inline bool GetKey(int key) { return m_input->GetKey(key);  }
		inline bool GetKeyPress(int key) { return m_input->GetKeyPress(key);  }
		inline bool GetKeyRelease(int key) { return m_input->GetKeyRelease(key);  }

		Engine();
		~Engine();
		void ProcessInputs(float dt);
		bool Initialize();
		void Update();
	};

}
#endif
