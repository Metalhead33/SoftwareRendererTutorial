#include "SoftwareRendererSystem.hpp"
#include "LoadImage.hpp"

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
	pipeline.vert = TexturedVertexShader;
	pipeline.frag = TexturedFragmentShader;
	pipeline.framebuffer = &renderBuffer;
	pipeline.uniform = { nullptr, DITHERED };
	pipeline.uniform.tex = loadImage("brickwall.png");
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

static const std::vector<TexturedVertexIn> vertices = {
	{ glm::vec3(-0.8f, 0.8f,0.f), glm::vec2(0.0f, 0.0f) }, // 0
	{ glm::vec3(-0.8f, -0.8f,0.f), glm::vec2(0.0f, 1.0f) }, // 1
	{ glm::vec3(0.8f, 0.8f,0.0f), glm::vec2(1.0f, 0.0f) }, // 2
	{ glm::vec3(0.8f, -0.8f,0.0f), glm::vec2(1.0f, 1.0f) } // 3
};
static const std::vector<unsigned> indices = {
	0, 1, 2,
	1, 2, 3
};

void SoftwareRendererSystem::render()
{
	pipeline.renderTriangles(vertices.data(),indices.data(), indices.size() );
	SDL_UpdateTexture(framebuffer.get(), nullptr, renderBuffer.getRawPixels(), renderBuffer.getStride() );
	SDL_RenderCopy(renderer.get(), framebuffer.get(), nullptr, nullptr);
	SDL_RenderPresent(renderer.get());
	SDL_Delay(16);
}
