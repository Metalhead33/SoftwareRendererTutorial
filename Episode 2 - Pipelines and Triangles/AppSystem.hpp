#ifndef APPSYSTEM_HPP
#define APPSYSTEM_HPP
#include <memory>
#include <string>
#include <SDL2/SDL.h>

class AppSystem
{
public:
	typedef std::unique_ptr<SDL_Window,decltype(&SDL_DestroyWindow)> uWindow;
private:
	AppSystem(const AppSystem& cpy) = delete; // Disable copy constructor
	AppSystem& operator=(const AppSystem& cpy) = delete; // Disable copy assignment operator
protected:
	uWindow window;
	virtual void processEvent(const SDL_Event& ev, bool& causesExit) = 0;
	virtual void updateLogic() = 0;
	virtual void render() = 0;
public:
	// Regular constructors
	virtual ~AppSystem() = default;
	AppSystem(const char *title, int offsetX, int offsetY, int width, int height, Uint32 flags);
	AppSystem(const std::string& title, int offsetX, int offsetY, int width, int height, Uint32 flags);
	void run();
};

#endif // APPSYSTEM_HPP
