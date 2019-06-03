
#include <Falcon.h>


class Game : public Falcon::Application {
public:
	Game() {};
	~Game() {};
};

int main()
{
	Falcon::Log::Init();
	FL_ENGINE_INFO("Hello I am from engine");
	
	Game * game = new Game();
	int a = 4;
	FL_GAME_WARN("my value={0}", a);
	game->Run();
	delete game;
}
