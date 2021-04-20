#ifndef MODELPIPELINE_HPP
#define MODELPIPELINE_HPP
#include <glm/glm.hpp>
#include "Texture.hpp"
#include "RenderingPipeline.hpp"
#include <memory>
#include "ZBuffer.hpp"

struct ModelUniform {
	ZBuffer* zbuff;
	std::shared_ptr<Texture> tex;
	TextureFiltering filtering;
	AlphaBlending blending;
	glm::mat4 model;
	glm::mat4 projection;
};
struct ModelVertexIn {
	glm::vec3 POS;
	glm::vec2 TEXCOORD;
	glm::vec4 COLOUR;
};
struct ModelVertexOut {
	glm::vec4 POS;
	glm::vec2 TEXCOORD;
	glm::vec4 COLOUR;
	inline static ModelVertexOut split(const ModelVertexOut& t, const ModelVertexOut& m, const ModelVertexOut& b, float dy, float iy) {
		return { glm::vec4(
						t.POS.x + ((b.POS.x - t.POS.x) / dy) * iy,
						m.POS.y,
						t.POS.z + ((b.POS.z - t.POS.z) / dy) * iy,
						t.POS.w + ((b.POS.w - t.POS.w) / dy) * iy
						),
					glm::vec2(
						t.TEXCOORD.r + ((b.TEXCOORD.r - t.TEXCOORD.r) / dy) * iy,
						t.TEXCOORD.g + ((b.TEXCOORD.g - t.TEXCOORD.g) / dy) * iy
						),
					glm::vec4(
						t.COLOUR.r + ((b.COLOUR.r - t.COLOUR.r) / dy) * iy,
						t.COLOUR.g + ((b.COLOUR.g - t.COLOUR.g) / dy) * iy,
						t.COLOUR.b + ((b.COLOUR.b - t.COLOUR.b) / dy) * iy,
						t.COLOUR.a + ((b.COLOUR.a - t.COLOUR.a) / dy) * iy
						) };
	}
	inline static ModelVertexOut interpolate(const ModelVertexOut& v0, const ModelVertexOut& v1, float w0, float w1, bool perspectiveCorrect) {
		ModelVertexOut out = {
							glm::vec4(  // POS
							(v0.POS.x * w0) + (v1.POS.x * w1), // X
							(v0.POS.y * w0) + (v1.POS.y * w1), // Y
							(v0.POS.z * w0) + (v1.POS.z * w1), // Z
							(v0.POS.w * w0) + (v1.POS.w * w1) // W
							),
							glm::vec2( // TEXCOORD
							(v0.TEXCOORD.x * w0) + (v1.TEXCOORD.x * w1), // X
							(v0.TEXCOORD.y * w0) + (v1.TEXCOORD.y * w1) // Y
							),
							glm::vec4( // COLOUR
							(v0.COLOUR.x * w0) + (v1.COLOUR.x * w1), // X
							(v0.COLOUR.y * w0) + (v1.COLOUR.y * w1), // Y
							(v0.COLOUR.z * w0) + (v1.COLOUR.z * w1), // Z
							(v0.COLOUR.w * w0) + (v1.COLOUR.w * w1) // W
							)
					};
		if(perspectiveCorrect)
		{
			out.TEXCOORD *= out.POS.w; // We're assuming that the TEXCOORD attribute of each vertex was divided by POS.z prior to calling this function
			out.COLOUR *= out.POS.w; // We're assuming that the COLOUR attribute of each vertex was divided by POS.z prior to calling this function
		}
		return out;
	}
	inline static ModelVertexOut interpolate(const ModelVertexOut& v0, const ModelVertexOut& v1, const ModelVertexOut& v2, float w0, float w1, float w2, bool perspectiveCorrect) {
		ModelVertexOut out = {
							glm::vec4(  // POS
							(v0.POS.x * w0) + (v1.POS.x * w1) + (v2.POS.x * w2), // X
							(v0.POS.y * w0) + (v1.POS.y * w1) + (v2.POS.y * w2), // Y
							(v0.POS.z * w0) + (v1.POS.z * w1) + (v2.POS.z * w2), // Z
							(v0.POS.w * w0) + (v1.POS.w * w1) + (v2.POS.w * w2) // W
							),
							glm::vec2( // TEXCOORD
							(v0.TEXCOORD.x * w0) + (v1.TEXCOORD.x * w1) + (v2.TEXCOORD.x * w2), // X
							(v0.TEXCOORD.y * w0) + (v1.TEXCOORD.y * w1) + (v2.TEXCOORD.y * w2) // Y
							),
							glm::vec4( // COLOUR
							(v0.COLOUR.x * w0) + (v1.COLOUR.x * w1) + (v2.COLOUR.x * w2), // X
							(v0.COLOUR.y * w0) + (v1.COLOUR.y * w1) + (v2.COLOUR.y * w2), // Y
							(v0.COLOUR.z * w0) + (v1.COLOUR.z * w1) + (v2.COLOUR.z * w2), // Z
							(v0.COLOUR.w * w0) + (v1.COLOUR.w * w1) + (v2.COLOUR.w * w2) // W
							)
					};
		if(perspectiveCorrect)
		{
			out.TEXCOORD *= out.POS.w; // We're assuming that the TEXCOORD attribute of each vertex was divided by POS.z prior to calling this function
			out.COLOUR *= out.POS.w; // We're assuming that the COLOUR attribute of each vertex was divided by POS.z prior to calling this function
		}
		return out;
	}
	void perspectiveCorrect() {
		TEXCOORD /= POS.w;
		COLOUR /= POS.w;
	}
};

typedef RenderingPipeline<ModelVertexIn,ModelVertexOut,ModelUniform> ModelPipeline;
ModelVertexOut ModelVertexShader(const ModelUniform& uniform, const ModelVertexIn& vertex);
void ModelFragmentShader(Texture& framebuffer, const glm::ivec2& point, const ModelUniform& uniform,
						 const ModelVertexOut& v0);

#endif // MODELPIPELINE_HPP
