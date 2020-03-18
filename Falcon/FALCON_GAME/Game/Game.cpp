#include "Game.h"

namespace gameLoop 
{
	Game::Game() :m_quitGame(false), m_initMenu(true), m_initGame(false), m_initPause(false),m_currentState(STATE::MENU) {}

	Game::~Game() 
	{
		delete m_engine;
	}

	bool Game::Init() 
	{
		m_engine = new gameLoop::Engine();
		

		if (!m_engine->Initialize()) return false;
		
		m_input = m_engine->GetInput();
		m_UI = m_engine->GetUI();
		return true;
	}

	void Game::Run() 
	{
		WindowClass* window= m_engine->GetWindow();
		while (!window->WindowCloseStatus())
		{
			switch (m_currentState) 
			{
			case STATE::MENU:
				if (m_initMenu)
				{
					Menu_Init();
					m_initMenu = false;
				}

				//Poll I/O events
				glfwPollEvents();

				Menu_Update();

				m_engine->MenuUpdate();
				break;

			case STATE::INGAME:
				if (m_initGame)
				{
					Game_Init();
				}

				//Poll I/O events
				glfwPollEvents();

				Game_Update();

				m_engine->IngameUpdate();
				break;

			case STATE::PAUSEMENU:
				if (m_initPause)
				{
					PauseMenu_Init();
				}
				//Poll I/O events
				glfwPollEvents();

				PauseMenu_Update();

				m_engine->PauseUpdate();
				break;

			default:
				break;
			}
		}
	}

	void Game::Game_Init() 
	{
	
	}

	void Game::Menu_Init() 
	{
		boost::function<void(void)> f1 = [&]() {m_UI->LoadUI("FIRST_PAGE"); };
		boost::function<void(void)> f2 = [&]() {m_UI->LoadUI("SECOND_PAGE"); };
		boost::function<void(void)> f3 = [&]() {m_UI->LoadUI("THIRD_PAGE"); };
		boost::function<void(void)> f4 = [&]() {m_initGame = true; m_currentState = STATE::INGAME; };

		//First Layer setup
		m_UI->AddImage("FIRST_PAGE", "start race.jpg", glm::vec4(0.0, 0.0, 1.0, 1.0));

		//Next Button
		m_UI->AddButton("FIRST_PAGE",
			glm::vec4(255, 255, 255, 0),
			glm::vec4(255, 255, 255, 255),
			glm::vec4(255, 255, 255, 255),
			glm::vec4(0.0f, 0.0f, 0.0f, 0.0f),
			glm::vec4(0.0f, 0.0f, 0.0f, 0.0f),
			glm::vec4(0.0f, 0.0f, 0.0f, 0.0f),
			glm::vec4(0.626, 0.820, 0.22, 0.122),
			"",
			f2
		);


		//Second Layer setup
		m_UI->AddImage("SECOND_PAGE", "choose track_lock.jpg", glm::vec4(0.0, 0.0, 1.0, 1.0));

		//next button
		m_UI->AddButton("SECOND_PAGE",
			glm::vec4(255, 255, 255, 0),
			glm::vec4(255, 255, 255, 255),
			glm::vec4(255, 255, 255, 255),
			glm::vec4(0.0f, 0.0f, 0.0f, 0.0f),
			glm::vec4(0.0f, 0.0f, 0.0f, 0.0f),
			glm::vec4(0.0f, 0.0f, 0.0f, 0.0f),
			glm::vec4(0.725, 0.883, 0.25, 0.08),
			"",
			f3
		);

		//prev button
		m_UI->AddButton("SECOND_PAGE",
			glm::vec4(255, 255, 255, 0),
			glm::vec4(255, 255, 255, 255),
			glm::vec4(255, 255, 255, 255),
			glm::vec4(0.0f, 0.0f, 0.0f, 0.0f),
			glm::vec4(0.0f, 0.0f, 0.0f, 0.0f),
			glm::vec4(0.0f, 0.0f, 0.0f, 0.0f),
			glm::vec4(glm::vec4(0.025, 0.883, 0.274, 0.081)),
			"",
			f1);

		//Third Layer Setup
		m_UI->AddImage("THIRD_PAGE", "tune car_1.jpg", glm::vec4(0.0, 0.0, 1.0, 1.0));

		//next button
		m_UI->AddButton("THIRD_PAGE",
			glm::vec4(255, 255, 255, 0),
			glm::vec4(255, 255, 255, 255),
			glm::vec4(255, 255, 255, 255),
			glm::vec4(0.0f, 0.0f, 0.0f, 0.0f),
			glm::vec4(0.0f, 0.0f, 0.0f, 0.0f),
			glm::vec4(0.0f, 0.0f, 0.0f, 0.0f),
			glm::vec4(0.725, 0.883, 0.25, 0.08),
			"",
			f4
		);

		//prev button
		m_UI->AddButton("THIRD_PAGE",
			glm::vec4(255, 255, 255, 0),
			glm::vec4(255, 255, 255, 255),
			glm::vec4(255, 255, 255, 255),
			glm::vec4(0.0f, 0.0f, 0.0f, 0.0f),
			glm::vec4(0.0f, 0.0f, 0.0f, 0.0f),
			glm::vec4(0.0f, 0.0f, 0.0f, 0.0f),
			glm::vec4(glm::vec4(0.025, 0.883, 0.274, 0.081)),
			"",
			f2);

		m_UI->AddSlider("THIRD_PAGE", glm::vec4(0.016, 0.62, 0.22, 0.1), 0.0f, 1.0f, 0.1f);
		m_UI->AddSlider("THIRD_PAGE", glm::vec4(0.258, 0.62, 0.22, 0.1), 0.0f, 1.0f, 0.1f);
		m_UI->AddSlider("THIRD_PAGE", glm::vec4(0.5, 0.62, 0.22, 0.1), 0.0f, 1.0f, 0.1f);

		m_UI->LoadUI("FIRST_PAGE");
	}

	void Game::PauseMenu_Init() 
	{
		
	}

	void Game::Menu_Update() 
	{
	
	}

	void Game::PauseMenu_Update() 
	{
		
	}

	void Game::Game_Update()
	{

	}
}