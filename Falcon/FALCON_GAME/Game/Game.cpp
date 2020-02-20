#include "Game.h"

namespace gameLoop 
{
	Game::Game() :m_quitGame(false),m_changeState(false), m_initMenu(true), m_initGame(false), m_initPause(false),m_currentState(STATE::INGAME) {}

	Game::~Game() 
	{
		delete m_engine;
	}

	bool Game::Init() 
	{
		m_engine = new gameLoop::Engine();
		if (!m_engine->Initialize()) return false;
		
		return true;
	}

	void Game::Run() 
	{
		WindowClass* window= m_engine->GetWindow();
		while (!window->WindowCloseStatus())
		{
			if (m_currentState == STATE::MENU)
			{
				if (m_initMenu) 
				{
					Menu_Init();
					m_initMenu = false;
				}

				//Poll I/O events
				glfwPollEvents();

				Menu_Update();

				m_engine->MenuUpdate();
			}
			else if (m_currentState == STATE::INGAME)
			{	
				if (m_initGame) 
				{
					Game_Init();
				}

				//Poll I/O events
				glfwPollEvents();

				Game_Update();

				m_engine->IngameUpdate();
			}
			else if (m_currentState == STATE::PAUSEMENU)
			{
				if (m_initPause) 
				{
					PauseMenu_Init();
				}
				//Poll I/O events
				glfwPollEvents();

				PauseMenu_Update();

				m_engine->PauseUpdate();
			}
		}
	}

	void Game::Game_Init() 
	{
	
	}

	void Game::Menu_Init() 
	{
	
	}

	void Game::PauseMenu_Init() 
	{
		
	}

	void Game::Game_Update() 
	{
	
	}

	void Game::Menu_Update() 
	{
	
	}

	void Game::PauseMenu_Update() 
	{
		
	}

	
}