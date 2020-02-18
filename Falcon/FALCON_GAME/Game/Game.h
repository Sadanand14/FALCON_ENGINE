#ifndef GAME_H
#define GAME_H

#include <System/Engine.h>
#include <System/Types.h>
#include "GameState.h"

namespace gameLoop
{
	class Game
	{
	private:
		boost::container::map<std::string, GameState*> m_states;
		Engine* m_engine = nullptr;
		bool m_quitGame, m_changeState;
		GameState* m_currentState;

	public:

		inline void AddState(GameState* state) 
		{ 
			if(m_states.find(state->m_title)==m_states.end())
			m_states[state->m_title] = state; 
		}
		bool Init();
		bool RunState(std::string stateName);
		Game();

		void Run();
		~Game();
	};

}
#endif // !GAME_H
