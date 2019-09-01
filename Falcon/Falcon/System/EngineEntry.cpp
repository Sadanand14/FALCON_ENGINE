#include "Application.h"

extern Application* CreateApplication();

int main()
{
	auto app = CreateApplication();
	app->Run();
	delete app;
}