#include <iostream>
#include "SoftwareRendererSystem.hpp"
using namespace std;
static const int WIDTH = 640;
static const int HEIGHT = 480;


int main()
{
	/*const rlim_t kStackSize = 16 * 1024 * 1024;   // min stack size = 16 MB
	struct rlimit rl;
	int result;
	result = getrlimit(RLIMIT_STACK, &rl);*/
	SDL_Init(SDL_INIT_VIDEO);
	SoftwareRendererSystem* app = new SoftwareRendererSystem(WIDTH,HEIGHT);
	app->run();
	delete app;
	return 0;
}
