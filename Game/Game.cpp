
#include <Falcon.h>


class Game : public Falcon::Application {
public:
	Game() {};
	~Game() {};
};

int main()
{
	Game * game = new Game();
	game->Run();
	delete game;
}
