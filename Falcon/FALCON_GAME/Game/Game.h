#ifndef GAME_H
#define GAME_H

#include <System/Engine.h>
#include <System/Types.h>

namespace gameLoop
{
	class Game
	{
	private:
		STATE m_currentState;
		Engine* m_engine = nullptr;
		bool m_quitGame, m_initMenu, m_initGame, m_initPause;
		InputReceiver* m_input = nullptr;

		inline bool GetKey(int key) { return m_input->GetKey(key); };
		inline bool GetKeyRelease(int key) {return m_input->GetKeyRelease(key);}
		inline bool GetKeyPress(int key) { return m_input->GetKeyPress(key); }


		void Game_Init();
		void Menu_Init();
		void PauseMenu_Init();

		void Game_Update();
		void Menu_Update();
		void PauseMenu_Update();

	public:

		bool Init();
		Game();

		void Run();
		~Game();
	};

}
#endif // !GAME_H
