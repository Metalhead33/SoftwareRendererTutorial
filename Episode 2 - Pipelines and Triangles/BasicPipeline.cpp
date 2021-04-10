#include "BasicPipeline.hpp"

BasicVertexOut basicVertexShader(const BasicUniform &uniform, const BasicVertexIn &vertex, const glm::ivec4 &viewport)
{
	const int viewportW = viewport[2] - viewport[0];
	const int viewportH = viewport[3] - viewport[1];
	return { glm::vec2(
					((vertex.POS[0] + 1.0f) / 2.0f * viewportW) + viewport[0] ,
					(((vertex.POS[1]-1.0f) / -2.0f) * viewportH) + viewport[1]
					), vertex.COLOUR };
}

void basicFragmentShader(Texture &framebuffer, const glm::ivec2 &point, const BasicUniform &uniform,
						 const BasicVertexOut &v0, const BasicVertexOut &v1, const BasicVertexOut &v2, float w0, float w1, float w2)
{
	if(point.x < 0 || point.y < 0) return;
	glm::vec4 colourKernel = {
					(w0 * v0.COLOUR.r) + (w1 * v1.COLOUR.r) + (w2 * v2.COLOUR.r),
					(w0 * v0.COLOUR.g) + (w1 * v1.COLOUR.g) + (w2 * v2.COLOUR.g),
					(w0 * v0.COLOUR.b) + (w1 * v1.COLOUR.b) + (w2 * v2.COLOUR.b),
					(w0 * v0.COLOUR.a) + (w1 * v1.COLOUR.a) + (w2 * v2.COLOUR.a)
					};
	framebuffer.setPixel(point,colourKernel);
}
