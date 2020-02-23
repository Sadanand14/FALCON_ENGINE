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

	enum class STATE{MENU, PAUSEMENU, INGAME};
	//struct LoopProperties
	//{
	//	bool m_enableRendering, m_enablePhysics, m_enableInput, m_enableAnimation, m_enableAudio, m_enableAI;

	//	LoopProperties() :m_enableRendering(true), m_enablePhysics(true), m_enableInput(true),
	//		m_enableAnimation(true), m_enableAudio(true), m_enableAI(true) {}

	//	inline void SetRendering(bool value) { m_enableRendering = value; }
	//	inline void SetPhysics(bool value) { m_enablePhysics = value; }
	//	inline void SetInput(bool value) { m_enableInput = value; }
	//	inline void SetAnimation(bool value) { m_enableAnimation = value; }
	//	inline void SetAudio(bool value) { m_enableAudio = value; }
	//	inline void SetAI(bool value) { m_enableAI = value; }
	//};

	/**
	* Main Game Class Responsible for owning and running all subsystems
	*/
	class Engine {

		//LoopProperties m_properties;
		Timer* m_timer;
		Renderer* m_renderer;
		WindowClass* m_window;
		InputReceiver* m_input;
		Scene::SceneGraph* m_scene;
		Rendering::Octree* m_octree;
		ParticleSystem* m_particleSystem = nullptr;
		STATE m_currentState;

		

	public:
		bool m_gameCrashed, m_windowClosed;
		//Audio Engine
		CAudioEngine m_audio;

		inline WindowClass* GetWindow() { return m_window; }
		inline InputReceiver* GetInput() { return m_input;  }
		inline bool GetKey(int key) { return m_input->GetKey(key);  }
		inline bool GetKeyPress(int key) { return m_input->GetKeyPress(key);  }
		inline bool GetKeyRelease(int key) { return m_input->GetKeyRelease(key);  }

		Engine();
		~Engine();
		void ProcessInputs(float dt);
		bool Initialize();
		void MenuUpdate();
		void PauseUpdate();
		void IngameUpdate();
	};

}
#endif
