#ifndef STANDARDPIXELTYPE_HPP
#define STANDARDPIXELTYPE_HPP
#include "StandardTexture.hpp"
#include <cstdint>

struct PixelRgba8 {
	uint32_t rgba;
	static constexpr const float reciprocal = 1.0f / 255.0f;
	inline void fillKernel(glm::vec4& colourKernel) const {
		colourKernel.r = float(rgba >> 24 & 0xFF) * reciprocal;
		colourKernel.g = float(rgba >> 16 & 0xFF) * reciprocal;
		colourKernel.b = float(rgba >> 8 & 0xFF) * reciprocal;
		colourKernel.a = float(rgba  & 0xFF) * reciprocal;
	}
	inline void fromKernel(const glm::vec4& colourKernel) {
		rgba = (uint32_t(colourKernel.r*255.0f) << 24) +
				(uint32_t(colourKernel.g*255.0f) << 16) +
				(uint32_t(colourKernel.b*255.0f) << 8) + uint32_t(colourKernel.a*255.0f);
	}
};
typedef StandardTexture<PixelRgba8> TextureRgba8;

#endif // STANDARDPIXELTYPE_HPP
