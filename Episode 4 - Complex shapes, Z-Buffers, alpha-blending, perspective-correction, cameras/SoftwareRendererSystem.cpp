#include "SoftwareRendererSystem.hpp"
#include "LoadImage.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <stdexcept>
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/mesh.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags

SoftwareRendererSystem::SoftwareRendererSystem(int width, int height)
	: AppSystem("Software Renderer Demo",0,0,width,height,0),
	renderer(SDL_CreateRenderer(this->window.get(),0,0),SDL_DestroyRenderer),
	framebuffer(SDL_CreateTexture(this->renderer.get(), SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, width,height),SDL_DestroyTexture),
	renderBuffer(width,height), zbuff(width,height)
{
	pipeline.viewport[0] = 0;
	pipeline.viewport[1] = 0;
	pipeline.viewport[2] = width;
	pipeline.viewport[3] = height;
	pipeline.vert = ModelVertexShader;
	pipeline.frag = ModelFragmentShader;
	//pipeline.vert = basicVertexShader;
	//pipeline.frag = basicFragmentShader;
	pipeline.clipper.znear = 0.1f;
	pipeline.perspectiveCorrection = false;
	pipeline.framebuffer = &renderBuffer;
	pipeline.uniform.tex = loadImage("vines.png");
	pipeline.uniform.filtering = NEAREST_NEIGHBOUR;
	pipeline.uniform.blending = ALPHA_DITHERING;
	//projection = glm::perspectiveFov(90.0f, float(width), float(height), 0.1f, 100.0f);
	projection = glm::frustum(-1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 100.0f);
	pipeline.uniform.zbuff = &zbuff;
	isFocused = true;
	CX = width / 2;
	CY = width / 2;
	mouseX = CX;
	mouseY = CY;
	pipeline.uniform.model = glm::mat4(1.0f);
	pipeline.uniform.model = glm::translate(pipeline.uniform.model, glm::vec3(0.0f, -1.75f, 0.0f)); // translate it down so it's at the center of the scene
	loadModel();
}

void SoftwareRendererSystem::processEvent(const SDL_Event &ev, bool &causesExit)
{
	switch(ev.type) {
	case SDL_QUIT: causesExit = true; break;
	case SDL_KEYDOWN: {
		switch(ev.key.keysym.sym) {
			case SDLK_ESCAPE: causesExit = true; break;
			case SDLK_w: camera.ProcessKeyboard(Camera::FORWARD, 0.016666666666667); break;
			case SDLK_s: camera.ProcessKeyboard(Camera::BACKWARD, 0.016666666666667); break;
			case SDLK_a: camera.ProcessKeyboard(Camera::LEFT, 0.016666666666667); break;
			case SDLK_d: camera.ProcessKeyboard(Camera::RIGHT, 0.016666666666667); break;
			default: break;
			}
			break;
		}
	case SDL_WINDOWEVENT: {
		switch(ev.window.event) {
			case SDL_WINDOWEVENT_FOCUS_GAINED: isFocused = true; break;
			case SDL_WINDOWEVENT_FOCUS_LOST: isFocused = false; break;
			default: break;
		}
		break;
	}
	default: break;
	}
}

void SoftwareRendererSystem::updateLogic()
{
	if(isFocused) {
	SDL_GetMouseState(&mouseX, &mouseY);
	SDL_WarpMouseInWindow(NULL,CX,CY);
	camera.ProcessMouseMovement(float(mouseX - CX),float((mouseY - CY) * -1),float(pipeline.viewport[2]),float(pipeline.viewport[3]),false);
	}
	pipeline.uniform.projection = projection *  camera.GetViewMatrix();
}

std::vector<BasicVertexIn> basicVertices = {
	{ glm::vec3( 0.0f, 1.0f, 0.5f ), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f) },
	{ glm::vec3( -1.0f, -1.0f, 0.5f ), glm::vec4(0.0f, 1.0f, 0.0f, 1.0f) },
	{ glm::vec3( 1.0f, -1.0f, 0.5f ), glm::vec4(0.0f,0.0f, 1.0f, 1.0f) }
};

void SoftwareRendererSystem::render()
{
	renderBuffer.clearToColour(glm::vec4(0.0f, 0.5f, 1.0f, 0.0f));
	zbuff.clear();
	//pipeline.renderTriangles(basicVertices.data(),basicVertices.size());
	pipeline.renderTriangles(vertices.data(),indices.data(), indices.size() );
	SDL_UpdateTexture(framebuffer.get(), nullptr, renderBuffer.getRawPixels(), renderBuffer.getStride() );
	SDL_RenderCopy(renderer.get(), framebuffer.get(), nullptr, nullptr);
	SDL_RenderPresent(renderer.get());
	SDL_Delay(16);
}

void SoftwareRendererSystem::loadModel()
{
	Assimp::Importer importer;
	importer.SetPropertyBool(AI_CONFIG_PP_PTV_NORMALIZE,true);
	const aiScene* scene = importer.ReadFile("cube.dae", aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices);
	if(!scene) throw std::runtime_error("Error loading the scene!");
	if(!scene->mNumMeshes) throw std::runtime_error("No models in the mesh!");
	const auto& mesh = *scene->mMeshes[0];
	if(!mesh.mNumVertices) throw std::runtime_error("There are no vertices in the mesh!!");
	if(!mesh.mNumFaces) throw std::runtime_error("There are no faces! in the mesh!!");
	if(!mesh.HasPositions()) throw std::runtime_error("There are no positions the mesh!!");
	if(!mesh.HasTextureCoords(0)) throw std::runtime_error("There are no texture coordinates the mesh!!");
	for(unsigned int i = 0; i < mesh.mNumVertices; ++i)
		{
			const auto& POS = mesh.mVertices[i];
			const auto& TEX = mesh.mTextureCoords[0][i];
			ModelVertexIn ver;
			ver.POS = { POS.x, POS.y, POS.z +2.0f };
			ver.TEXCOORD = { TEX.x, TEX.y };
			ver.COLOUR = glm::vec4(1.0f, 1.0f, 1.0f, 0.75f);
			/*switch (i % 3) {
			case 0:
				ver.COLOUR = glm::vec4(1.0f, 0.5f, 0.5f, 0.5f);
				break;
			case 1:
				ver.COLOUR = glm::vec4(0.5f, 1.0f, 0.5f, 0.5f);
				break;
			case 2:
				ver.COLOUR = glm::vec4(0.5f, 0.5f, 1.0f, 0.5f);
				break;
			default:
				ver.COLOUR = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
				break;
			}*/
			vertices.push_back(ver);
		}
		for(unsigned int i = 0; i < mesh.mNumFaces; ++i)
		{
			const auto& face = mesh.mFaces[i];
			for(unsigned int j = 0; j < face.mNumIndices; ++j) indices.push_back(face.mIndices[j]);
		}
}
