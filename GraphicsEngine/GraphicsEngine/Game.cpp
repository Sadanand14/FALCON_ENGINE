#include "game.h"
#include <iostream>


int main()
{
	WindowClass* window1 = new WindowClass("mywindow", 1280, 720);
	
	while(true)
	{
		window1->Update();
	}
	return 0;
}
