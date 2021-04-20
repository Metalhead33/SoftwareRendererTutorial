#include "BasicPipeline.hpp"

BasicVertexOut basicVertexShader(const BasicUniform &uniform, const BasicVertexIn &vertex)
{
	BasicVertexOut out = { glm::vec4(vertex.POS,0.5f, 1.0f) , vertex.COLOUR };
	return out;
}

void basicFragmentShader(Texture &framebuffer, const glm::ivec2 &point, const BasicUniform &uniform,
						 const BasicVertexOut &v0)
{
	if(point.x < 0 || point.y < 0) return;
	framebuffer.setPixelDithered(point,point,v0.COLOUR);
}
