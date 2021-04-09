#include "SoftwareRendererSystem.hpp"


SoftwareRendererSystem::SoftwareRendererSystem(int width, int height)
	: AppSystem("Software Renderer Demo",0,0,width,height,0),
	renderer(SDL_CreateRenderer(this->window.get(),0,0),SDL_DestroyRenderer),
	framebuffer(SDL_CreateTexture(this->renderer.get(), SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, width,height),SDL_DestroyTexture),
	renderBuffer(width,height)
{

}

void SoftwareRendererSystem::processEvent(const SDL_Event &ev, bool &causesExit)
{
	switch(ev.type) {
	case SDL_QUIT: causesExit = true; break;
	default: break;
	}
}

void SoftwareRendererSystem::updateLogic()
{

}

void SoftwareRendererSystem::render()
{
	for(int x = 0; x < renderBuffer.getWidth(); ++x) {
		for(int y = 0; y < renderBuffer.getHeight(); ++y) {
			const float r = float(x) / renderBuffer.getWidthF();
			const float g = float(y) / renderBuffer.getHeightF();
			renderBuffer.setPixel(glm::ivec2(x,y),glm::vec4(r,g,0.0f,1.0f));
		}
	}
	SDL_UpdateTexture(framebuffer.get(), nullptr, renderBuffer.getRawPixels(), renderBuffer.getStride() );
	SDL_RenderCopy(renderer.get(), framebuffer.get(), nullptr, nullptr);
	SDL_RenderPresent(renderer.get());
}
