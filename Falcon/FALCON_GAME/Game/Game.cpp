#include "Game.h"

namespace gameLoop 
{
	Game::Game() :m_quitGame(false),m_changeState(false), m_currentState(nullptr) {}

	Game::~Game() 
	{
		fmemory::fdelete(m_engine);
	}

	bool Game::Init() 
	{
		m_engine = new gameLoop::Engine();
		if (!m_engine->Initialize()) return false;
		
		return true;
	}

	bool Game::RunState(std::string stateName)
	{
		m_changeState = true;
		if (m_states.find(stateName) == m_states.end()) return false;

		m_currentState = m_states[stateName];
		return true;
	}

	void Game::Run() 
	{
		while (!m_quitGame)
		{
			//m_currentState->Start();

			while(!m_changeState)
			{
				m_engine->Update();
			}
		}
	}
}