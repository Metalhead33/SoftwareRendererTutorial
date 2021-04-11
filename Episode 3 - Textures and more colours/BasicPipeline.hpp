#ifndef BASICPIPELINE_HPP
#define BASICPIPELINE_HPP
#include <glm/glm.hpp>
#include "Texture.hpp"
#include "RenderingPipeline.hpp"

struct BasicUniform {
	int dummy; // I'm only here because something has to be.
};
struct BasicVertexIn {
	glm::vec3 POS;
	glm::vec4 COLOUR;
};
struct BasicVertexOut {
	glm::vec2 POS;
	glm::vec4 COLOUR;
	inline static BasicVertexOut split(const BasicVertexOut& t, const BasicVertexOut& m, const BasicVertexOut& b, float dy, float iy) {
		return { glm::vec2(
						t.POS.x + ((b.POS.x - t.POS.x) / dy) * iy,
						m.POS.y
						),
					glm::vec4(
						t.COLOUR.r + ((b.COLOUR.r - t.COLOUR.r) / dy) * iy,
						t.COLOUR.g + ((b.COLOUR.g - t.COLOUR.g) / dy) * iy,
						t.COLOUR.b + ((b.COLOUR.b - t.COLOUR.b) / dy) * iy,
						t.COLOUR.a + ((b.COLOUR.a - t.COLOUR.a) / dy) * iy
						) };
	}
};

typedef RenderingPipeline<BasicVertexIn,BasicVertexOut,BasicUniform> BasicPipeline;
BasicVertexOut basicVertexShader(const BasicUniform& uniform, const BasicVertexIn& vertex, const glm::ivec4& viewport);
void basicFragmentShader(Texture& framebuffer, const glm::ivec2& point, const BasicUniform& uniform,
						 const BasicVertexOut& v0,const BasicVertexOut& v1, const BasicVertexOut& v2, float w0, float w1, float w2);


#endif // BASICPIPELINE_HPP
