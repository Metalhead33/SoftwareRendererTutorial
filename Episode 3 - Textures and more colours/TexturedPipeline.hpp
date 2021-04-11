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
	glm::vec3 POS;
	glm::vec2 TEXCOORD;
};
struct TexturedVertexOut {
	glm::vec2 POS;
	glm::vec2 TEXCOORD;
	inline static TexturedVertexOut split(const TexturedVertexOut& t, const TexturedVertexOut& m, const TexturedVertexOut& b, float dy, float iy) {
		return { glm::vec2(
						t.POS.x + ((b.POS.x - t.POS.x) / dy) * iy,
						m.POS.y
						),
					glm::vec2(
						t.TEXCOORD.r + ((b.TEXCOORD.r - t.TEXCOORD.r) / dy) * iy,
						t.TEXCOORD.g + ((b.TEXCOORD.g - t.TEXCOORD.g) / dy) * iy
						) };
	}
};

typedef RenderingPipeline<TexturedVertexIn,TexturedVertexOut,TexturedUniform> TexturedPipeline;
TexturedVertexOut TexturedVertexShader(const TexturedUniform& uniform, const TexturedVertexIn& vertex, const glm::ivec4& viewport);
void TexturedFragmentShader(Texture& framebuffer, const glm::ivec2& point, const TexturedUniform& uniform,
						 const TexturedVertexOut& v0,const TexturedVertexOut& v1, const TexturedVertexOut& v2, float w0, float w1, float w2);

#endif // TEXTUREDPIPELINE_HPP
