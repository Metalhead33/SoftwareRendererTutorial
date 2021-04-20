#ifndef BASICPIPELINE_HPP
#define BASICPIPELINE_HPP
#include <glm/glm.hpp>
#include "Texture.hpp"
#include "RenderingPipeline.hpp"

struct BasicUniform {
	int dummy; // I'm only here because something has to be.
};
struct BasicVertexIn {
	glm::vec2 POS;
	glm::vec4 COLOUR;
};
struct BasicVertexOut {
	glm::vec4 POS;
	glm::vec4 COLOUR;
	inline static BasicVertexOut split(const BasicVertexOut& t, const BasicVertexOut& m, const BasicVertexOut& b, float dy, float iy) {
		return { glm::vec4(
						t.POS.x + ((b.POS.x - t.POS.x) / dy) * iy,
						m.POS.y,
						t.POS.z + ((b.POS.z - t.POS.z) / dy) * iy,
						t.POS.w + ((b.POS.w - t.POS.w) / dy) * iy
						),
					glm::vec4(
						t.COLOUR.r + ((b.COLOUR.r - t.COLOUR.r) / dy) * iy,
						t.COLOUR.g + ((b.COLOUR.g - t.COLOUR.g) / dy) * iy,
						t.COLOUR.b + ((b.COLOUR.b - t.COLOUR.b) / dy) * iy,
						t.COLOUR.a + ((b.COLOUR.a - t.COLOUR.a) / dy) * iy
						) };
	}
	inline static BasicVertexOut interpolate(const BasicVertexOut& v0, const BasicVertexOut& v1, float w0, float w1, bool perspectiveCorrect) {
		BasicVertexOut out = {
							glm::vec4(  // POS
							(v0.POS.x * w0) + (v1.POS.x * w1), // X
							(v0.POS.y * w0) + (v1.POS.y * w1), // Y
							(v0.POS.z * w0) + (v1.POS.z * w1), // Z
							(v0.POS.w * w0) + (v1.POS.w * w1) // W
							),
							glm::vec4( // COLOUR
							(v0.COLOUR.x * w0) + (v1.COLOUR.x * w1), // X
							(v0.COLOUR.y * w0) + (v1.COLOUR.y * w1), // Y
							(v0.COLOUR.z * w0) + (v1.COLOUR.z * w1), // Z
							(v0.COLOUR.w * w0) + (v1.COLOUR.w * w1) // W
							)
					};
		if(perspectiveCorrect) out.COLOUR *= out.POS.w; // We're assuming that the COLOUR attribute of each vertex was divided by POS.z prior to calling this function
		return out;
	}
	inline static BasicVertexOut interpolate(const BasicVertexOut& v0, const BasicVertexOut& v1, const BasicVertexOut& v2, float w0, float w1, float w2, bool perspectiveCorrect) {
		BasicVertexOut out = {
							glm::vec4(  // POS
							(v0.POS.x * w0) + (v1.POS.x * w1) + (v2.POS.x * w2), // X
							(v0.POS.y * w0) + (v1.POS.y * w1) + (v2.POS.y * w2), // Y
							(v0.POS.z * w0) + (v1.POS.z * w1) + (v2.POS.z * w2), // Z
							(v0.POS.w * w0) + (v1.POS.w * w1) + (v2.POS.w * w2) // W
							),
							glm::vec4( // COLOUR
							(v0.COLOUR.x * w0) + (v1.COLOUR.x * w1) + (v2.COLOUR.x * w2), // X
							(v0.COLOUR.y * w0) + (v1.COLOUR.y * w1) + (v2.COLOUR.y * w2), // Y
							(v0.COLOUR.z * w0) + (v1.COLOUR.z * w1) + (v2.COLOUR.z * w2), // Z
							(v0.COLOUR.w * w0) + (v1.COLOUR.w * w1) + (v2.COLOUR.w * w2) // W
							)
					};
		if(perspectiveCorrect) out.COLOUR *= out.POS.w; // We're assuming that the COLOUR attribute of each vertex was divided by POS.z prior to calling this function
		return out;
	}
	void perspectiveCorrect() {
		COLOUR /= POS.w;
	}
};

typedef RenderingPipeline<BasicVertexIn,BasicVertexOut,BasicUniform> BasicPipeline;
BasicVertexOut basicVertexShader(const BasicUniform& uniform, const BasicVertexIn& vertex);
void basicFragmentShader(Texture& framebuffer, const glm::ivec2& point, const BasicUniform& uniform, const BasicVertexOut& v0);


#endif // BASICPIPELINE_HPP
