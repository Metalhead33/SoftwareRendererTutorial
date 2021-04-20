#ifndef TEXTUREDPIPELINE_HPP
#define TEXTUREDPIPELINE_HPP
#include <glm/glm.hpp>
#include "Texture.hpp"
#include "RenderingPipeline.hpp"
#include <memory>

struct TexturedUniform {
	std::shared_ptr<Texture> tex;
	TextureFiltering filtering;
};
struct TexturedVertexIn {
	glm::vec2 POS;
	glm::vec2 TEXCOORD;
};
struct TexturedVertexOut {
	glm::vec4 POS;
	glm::vec2 TEXCOORD;
	inline static TexturedVertexOut split(const TexturedVertexOut& t, const TexturedVertexOut& m, const TexturedVertexOut& b, float dy, float iy) {
		return { glm::vec4(
						t.POS.x + ((b.POS.x - t.POS.x) / dy) * iy,
						m.POS.y,
						t.POS.z + ((b.POS.z - t.POS.z) / dy) * iy,
						t.POS.w + ((b.POS.w - t.POS.w) / dy) * iy
						),
					glm::vec2(
						t.TEXCOORD.r + ((b.TEXCOORD.r - t.TEXCOORD.r) / dy) * iy,
						t.TEXCOORD.g + ((b.TEXCOORD.g - t.TEXCOORD.g) / dy) * iy
						) };
	}
	inline static TexturedVertexOut interpolate(const TexturedVertexOut& v0, const TexturedVertexOut& v1, float w0, float w1, bool perspectiveCorrect) {
		TexturedVertexOut out = {
							glm::vec4(  // POS
							(v0.POS.x * w0) + (v1.POS.x * w1), // X
							(v0.POS.y * w0) + (v1.POS.y * w1), // Y
							(v0.POS.z * w0) + (v1.POS.z * w1), // Z
							(v0.POS.w * w0) + (v1.POS.w * w1) // Z
							),
							glm::vec2( // TEXCOORD
							(v0.TEXCOORD.x * w0) + (v1.TEXCOORD.x * w1), // X
							(v0.TEXCOORD.y * w0) + (v1.TEXCOORD.y * w1) // Y
							)
					};
		if(perspectiveCorrect) out.TEXCOORD *= out.POS.w; // We're assuming that the TEXCOORD attribute of each vertex was divided by POS.z prior to calling this function
		return out;
	}
	inline static TexturedVertexOut interpolate(const TexturedVertexOut& v0, const TexturedVertexOut& v1, const TexturedVertexOut& v2, float w0, float w1, float w2, bool perspectiveCorrect) {
		TexturedVertexOut out = {
							glm::vec4(  // POS
							(v0.POS.x * w0) + (v1.POS.x * w1) + (v2.POS.x * w2), // X
							(v0.POS.y * w0) + (v1.POS.y * w1) + (v2.POS.y * w2), // Y
							(v0.POS.z * w0) + (v1.POS.z * w1) + (v2.POS.z * w2), // Z
							(v0.POS.w * w0) + (v1.POS.w * w1) + (v2.POS.w * w2) // Z
							),
							glm::vec2( // TEXCOORD
							(v0.TEXCOORD.x * w0) + (v1.TEXCOORD.x * w1) + (v2.TEXCOORD.x * w2), // X
							(v0.TEXCOORD.y * w0) + (v1.TEXCOORD.y * w1) + (v2.TEXCOORD.y * w2) // Y
							)
					};
		if(perspectiveCorrect) out.TEXCOORD *= out.POS.w; // We're assuming that the TEXCOORD attribute of each vertex was divided by POS.z prior to calling this function
		return out;
	}
	void perspectiveCorrect() {
		TEXCOORD /= POS.z;
	}
};

typedef RenderingPipeline<TexturedVertexIn,TexturedVertexOut,TexturedUniform> TexturedPipeline;
TexturedVertexOut TexturedVertexShader(const TexturedUniform& uniform, const TexturedVertexIn& vertex);
void TexturedFragmentShader(Texture& framebuffer, const glm::ivec2& point, const TexturedUniform& uniform,
						 const TexturedVertexOut& v0);

#endif // TEXTUREDPIPELINE_HPP
