#ifndef SOFTWARERENDERERSYSTEM_HPP
#define SOFTWARERENDERERSYSTEM_HPP
#include "AppSystem.hpp"
#include "StandardPixelType.hpp"
#include "BasicPipeline.hpp"

class SoftwareRendererSystem : public AppSystem
{
public:
	typedef std::unique_ptr<SDL_Texture,decltype(&SDL_DestroyTexture)> uSdlTexture;
	typedef std::unique_ptr<SDL_Renderer,decltype(&SDL_DestroyRenderer)> uSdlRenderer;
protected:
	uSdlRenderer renderer;
	uSdlTexture framebuffer;
	TextureRgba8 renderBuffer;
	BasicPipeline pipeline;
	void processEvent(const SDL_Event& ev, bool& causesExit);
	void updateLogic();
	void render();
public:
	SoftwareRendererSystem(int width, int height);
};

#endif // SOFTWARERENDERERSYSTEM_HPP
