#include <iostream>
#include "SoftwareRendererSystem.hpp"


using namespace std;
static const int WIDTH = 640;
static const int HEIGHT = 480;

int main()
{
	SDL_Init(SDL_INIT_VIDEO);
	SoftwareRendererSystem app(WIDTH,HEIGHT);
	app.run();
	return 0;
}
