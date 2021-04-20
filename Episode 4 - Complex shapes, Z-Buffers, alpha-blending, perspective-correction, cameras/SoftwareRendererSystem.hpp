#ifndef SOFTWARERENDERERSYSTEM_HPP
#define SOFTWARERENDERERSYSTEM_HPP
#include "AppSystem.hpp"
#include "StandardPixelType.hpp"
#include "BasicPipeline.hpp"
#include "TexturedPipeline.hpp"
#include "ModelPipeline.hpp"
#include "Camera.hpp"

class SoftwareRendererSystem : public AppSystem
{
public:
	typedef std::unique_ptr<SDL_Texture,decltype(&SDL_DestroyTexture)> uSdlTexture;
	typedef std::unique_ptr<SDL_Renderer,decltype(&SDL_DestroyRenderer)> uSdlRenderer;
protected:
	uSdlRenderer renderer;
	uSdlTexture framebuffer;
	TextureRgba8 renderBuffer;
	ZBuffer zbuff;
	//BasicPipeline pipeline;
	ModelPipeline pipeline;
	Camera camera;
	void processEvent(const SDL_Event& ev, bool& causesExit);
	void updateLogic();
	void render();
	bool isFocused;
	int mouseX, mouseY;
	int CX;
	int CY;
	glm::mat4 projection;
	std::vector<ModelVertexIn> vertices;
	std::vector<unsigned> indices;
	void loadModel();
public:
	SoftwareRendererSystem(int width, int height);
};

#endif // SOFTWARERENDERERSYSTEM_HPP
