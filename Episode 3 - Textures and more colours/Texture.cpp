#include "Texture.hpp"
#include <cmath>

static const glm::vec2 LOOKUP[2][2] = {
{  glm::vec2( 0.25f, 0.00f ), glm::vec2( 0.50f, 0.75f ) },
{  glm::vec2( 0.75f, 0.50f ), glm::vec2( 0.00f, 0.25f ) }
};

void Texture::sample(const glm::vec2 &pos, const glm::ivec2 &screenpos, TextureFiltering filteringType, glm::vec4 &colourKernel) const
{
	switch (filteringType) {
	case NEAREST_NEIGHBOUR:
		getPixel(glm::ivec2( int( std::round(pos.x*getWidthF())) % getWidth() , int(std::round(pos.y * getHeightF())) % getHeight() ),
				 colourKernel);
		break;
	case DITHERED:
		{
		glm::vec2 texelCoords = glm::vec2(pos.x * getWidthF(),pos.y * getHeightF());
		texelCoords += LOOKUP[screenpos[1]&1][screenpos[0]&1];
		getPixel(glm::ivec2( int(std::round(texelCoords.x))%getWidth(),int(std::round(texelCoords.y))%getHeight() ),colourKernel);
		break;
		}
	case BILINEAR:
		{
		const int w = getWidth();
		const int h = getHeight();
		const glm::vec2 tmp = glm::vec2(pos.x * getWidthF(),pos.y * getHeightF() );
		const glm::vec2 coordEdgeTopLeft( std::floor(tmp[0]), std::floor(tmp[1]) );
		const glm::vec2 coordEdgeTopRight( std::ceil(tmp[0]), std::floor(tmp[1]) );
		const glm::vec2 coordEdgeBottomLeft( std::floor(tmp[0]), std::ceil(tmp[1]) );
		const glm::vec2 coordEdgeBottomRight( std::ceil(tmp[0]), std::ceil(tmp[1]) );
		const glm::vec2 weight = tmp - coordEdgeTopLeft;
		glm::vec4 colourTopLeft, colourTopRight, colourBottomLeft, colourBottomRight;
		getPixel(glm::ivec2( int(coordEdgeTopLeft[0]) % w,int(coordEdgeTopLeft[1]) % h ),colourTopLeft );
		getPixel(glm::ivec2( int(coordEdgeTopRight[0]) % w,int(coordEdgeTopRight[1]) % h ),colourTopRight );
		getPixel(glm::ivec2( int(coordEdgeBottomLeft[0]) % w,int(coordEdgeBottomLeft[1]) % h ),colourBottomLeft );
		getPixel(glm::ivec2( int(coordEdgeBottomRight[0]) % w,int(coordEdgeBottomRight[1]) % h ),colourBottomRight );
		colourTopLeft *= ((1.0f-weight[0]) * (1.0f-weight[1]));
		colourTopRight *= (weight[0] * (1.0f-weight[1]));
		colourBottomLeft *= ((1.0f-weight[0]) * weight[1]);
		colourBottomRight *= (weight[0] * weight[1]);
		colourKernel = colourTopLeft + colourTopRight + colourBottomLeft + colourBottomRight;
		break;
		}
	default:
		break;
	}
}
