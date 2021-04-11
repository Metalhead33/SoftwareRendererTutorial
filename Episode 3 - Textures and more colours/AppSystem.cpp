#include "AppSystem.hpp"

AppSystem::AppSystem(const char *title, int offsetX, int offsetY, int width, int height, Uint32 flags)
	: window(SDL_CreateWindow(title,offsetX,offsetY,width,height,flags),SDL_DestroyWindow)
{

}

AppSystem::AppSystem(const std::string &title, int offsetX, int offsetY, int width, int height, Uint32 flags)
	: window(SDL_CreateWindow(title.c_str(),offsetX,offsetY,width,height,flags),SDL_DestroyWindow)
{

}

void AppSystem::run()
{
	bool isInterrupted=false;
	do {
		SDL_Event ev;
		while(SDL_PollEvent(&ev)) {
			processEvent(ev,isInterrupted);
		}
		updateLogic();
		render();
	} while(!isInterrupted);
}
