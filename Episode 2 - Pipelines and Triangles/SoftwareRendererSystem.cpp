#include "SoftwareRendererSystem.hpp"


SoftwareRendererSystem::SoftwareRendererSystem(int width, int height)
	: AppSystem("Software Renderer Demo",0,0,width,height,0),
	renderer(SDL_CreateRenderer(this->window.get(),0,0),SDL_DestroyRenderer),
	framebuffer(SDL_CreateTexture(this->renderer.get(), SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, width,height),SDL_DestroyTexture),
	renderBuffer(width,height)
{
	pipeline.viewport[0] = 0;
	pipeline.viewport[1] = 0;
	pipeline.viewport[2] = width;
	pipeline.viewport[3] = height;
	pipeline.vert = basicVertexShader;
	pipeline.frag = basicFragmentShader;
	pipeline.framebuffer = &renderBuffer;
	pipeline.uniform = { 0 };
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

static const BasicVertexIn vertices[] = {
	{ glm::vec3(0.0f, 1.0f,0.f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f) },
	{ glm::vec3(-1.0f, -1.0f,0.0f), glm::vec4(0.0f, 1.0f, 0.0f, 1.0f) },
	{ glm::vec3(1.0f, -0.8f,0.0f), glm::vec4(0.0f, 0.0f, 1.0f, 1.0f) }
};

void SoftwareRendererSystem::render()
{
	pipeline.renderTriangle(vertices[0],vertices[1],vertices[2]);
	SDL_UpdateTexture(framebuffer.get(), nullptr, renderBuffer.getRawPixels(), renderBuffer.getStride() );
	SDL_RenderCopy(renderer.get(), framebuffer.get(), nullptr, nullptr);
	SDL_RenderPresent(renderer.get());
	SDL_Delay(16);
}
