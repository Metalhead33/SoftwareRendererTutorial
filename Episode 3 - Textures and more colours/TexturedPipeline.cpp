#include "TexturedPipeline.hpp"

TexturedVertexOut TexturedVertexShader(const TexturedUniform &uniform, const TexturedVertexIn &vertex, const glm::ivec4 &viewport)
{
	const int viewportW = viewport[2] - viewport[0];
	const int viewportH = viewport[3] - viewport[1];
	return { glm::vec2(
					((vertex.POS[0] + 1.0f) / 2.0f * viewportW) + viewport[0] ,
					(((vertex.POS[1]-1.0f) / -2.0f) * viewportH) + viewport[1]
					), vertex.TEXCOORD };
}

void TexturedFragmentShader(Texture &framebuffer, const glm::ivec2 &point, const TexturedUniform &uniform,
						 const TexturedVertexOut &v0, const TexturedVertexOut &v1, const TexturedVertexOut &v2, float w0, float w1, float w2)
{
	if(point.x < 0 || point.y < 0) return;
	if(!uniform.tex) return;
	glm::vec2 texCoord = {
					(w0 * v0.TEXCOORD.r) + (w1 * v1.TEXCOORD.r) + (w2 * v2.TEXCOORD.r),
					(w0 * v0.TEXCOORD.g) + (w1 * v1.TEXCOORD.g) + (w2 * v2.TEXCOORD.g)
					};
	framebuffer.setPixelDithered(point,point,uniform.tex->sample(texCoord,point,uniform.filtering));
}
