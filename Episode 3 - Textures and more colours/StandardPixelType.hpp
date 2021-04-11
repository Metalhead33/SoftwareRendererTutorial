#ifndef STANDARDPIXELTYPE_HPP
#define STANDARDPIXELTYPE_HPP
#include "StandardTexture.hpp"
#include <cstdint>
#include "Normalize.hpp"
#include "Dither.hpp"

struct PixelRgba8 {
	uint32_t rgba;
	typedef OrderedDither<uint8_t,std::numeric_limits<uint8_t>::max()> Dither;
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
	inline void fromKernelDithered(const glm::vec4& colourKernel, const glm::ivec2& screencoord) {
		rgba = (uint32_t(Dither::ditherDown(colourKernel.r,screencoord)*255.0f) << 24) +
				(uint32_t(Dither::ditherDown(colourKernel.g,screencoord)*255.0f) << 16) +
				(uint32_t(Dither::ditherDown(colourKernel.b,screencoord)*255.0f) << 8) + uint32_t(colourKernel.a*255.0f);
	}
};
typedef StandardTexture<PixelRgba8> TextureRgba8;

struct PixelRgb565 {
	uint16_t rgb;
	typedef OrderedDither<uint8_t,31> Dither5;
	typedef OrderedDither<uint8_t,63> Dither6;
	static constexpr const float i5r = 1.0f / 31.0f;
	static constexpr const float i6r = 1.0f / 63.0f;
	static constexpr const float reciprocal = 1.0f / 255.0f;
	inline void fillKernel(glm::vec4& colourKernel) const {
		const uint16_t r = (rgb & 0xF800) >> 11;
		const uint16_t g = (rgb & 0x07E0) >> 5;
		const uint16_t b = (rgb & 0x001F);
		colourKernel.r = float(r) * i5r;
		colourKernel.g = float(g) * i6r;
		colourKernel.b = float(b) * i5r;
		colourKernel.a = 1.0f;
	}
	inline void fromKernel(const glm::vec4& colourKernel) {
		const uint16_t r = uint16_t(colourKernel.r * 31.0f);
		const uint16_t g = uint16_t(colourKernel.g * 63.0f);
		const uint16_t b = uint16_t(colourKernel.b * 31.0f);
		rgb = ((r << 11) | (g << 5) | b);
	}
	inline void fromKernelDithered(const glm::vec4& colourKernel, const glm::ivec2& screencoord) {
		const uint16_t r = uint16_t(Dither5::ditherDown(colourKernel.r,screencoord)  * 31.0f);
		const uint16_t g = uint16_t(Dither6::ditherDown(colourKernel.g,screencoord) * 63.0f);
		const uint16_t b = uint16_t(Dither5::ditherDown(colourKernel.b,screencoord) * 31.0f);
		rgb = ((r << 11) | (g << 5) | b);
	}
};
typedef StandardTexture<PixelRgb565> TextureRgb565;

struct PixelRgb332 {
	uint8_t rgb;
	typedef OrderedDither<uint8_t,3> Dither2;
	typedef OrderedDither<uint8_t,7> Dither3;
	static constexpr const float i2r = 1.0f / 3.0f;
	static constexpr const float i3r = 1.0f / 7.0f;
	inline void fillKernel(glm::vec4& colourKernel) const {
		const uint8_t r = (rgb & 0xE0) >> 5;
		const uint8_t g = (rgb & 0x1C) >> 2;
		const uint8_t b = (rgb & 0x03);
		colourKernel.r = float(r) * i3r;
		colourKernel.g = float(g) * i3r;
		colourKernel.b = float(b) * i2r;
		colourKernel.a = 1.0f;
	}
	inline void fromKernel(const glm::vec4& colourKernel) {
		const uint16_t r = uint16_t(colourKernel.r * 7.0f);
		const uint16_t g = uint16_t(colourKernel.g * 7.0f);
		const uint16_t b = uint16_t(colourKernel.b * 3.0f);
		rgb = ((r << 5) | (g << 2) | b);
	}
	inline void fromKernelDithered(const glm::vec4& colourKernel, const glm::ivec2& screencoord) {
		const uint16_t r = uint16_t(Dither3::ditherDown(colourKernel.r,screencoord)  * 7.0f);
		const uint16_t g = uint16_t(Dither3::ditherDown(colourKernel.g,screencoord) * 7.0f);
		const uint16_t b = uint16_t(Dither2::ditherDown(colourKernel.b,screencoord) * 3.0f);
		rgb = ((r << 5) | (g << 2) | b);
	}
};
typedef StandardTexture<PixelRgb332> TextureRgb332;

template <typename T> struct StdPixelGreyscale {
	T val;
	typedef OrderedDither<T,std::numeric_limits<T>::max()> Dither;
	inline void fillKernel(glm::vec4& colourKernel) const {
		const float norm = normalize(val);
		colourKernel.r = norm;
		colourKernel.g = norm;
		colourKernel.b = norm;
		colourKernel.a = 1.0f;
	}
	inline void fromKernel(const glm::vec4& colourKernel) {
		val = denormalize<T>( (0.2989f * colourKernel.r) +
							(0.5870f * colourKernel.g) +
							( 0.1140f * colourKernel.b) );
	}
	inline void fromKernelDithered(const glm::vec4& colourKernel, const glm::ivec2& screencoord) {
		val = denormalize<T>(Dither::ditherDown(
								 ( (0.2989f * colourKernel.r) +
															 (0.5870f * colourKernel.g) +
															 ( 0.1140f * colourKernel.b) )
								 ,screencoord));
	}
};
template <typename T> struct StdPixelRgb {
	typedef OrderedDither<T,std::numeric_limits<T>::max()> Dither;
	T r,g,b;
	inline void fillKernel(glm::vec4& colourKernel) const {
		colourKernel.r = normalize(r);
		colourKernel.g = normalize(g);
		colourKernel.b = normalize(b);
		colourKernel.a = 1.0f;
	}
	inline void fromKernel(const glm::vec4& colourKernel) {
		r = denormalize<T>(colourKernel.r);
		g = denormalize<T>(colourKernel.g);
		b = denormalize<T>(colourKernel.b);
	}
	inline void fromKernelDithered(const glm::vec4& colourKernel, const glm::ivec2& screencoord) {
		r = denormalize<T>(Dither::ditherDown(colourKernel.r,screencoord));
		g = denormalize<T>(Dither::ditherDown(colourKernel.g,screencoord));
		b = denormalize<T>(Dither::ditherDown(colourKernel.b,screencoord));
	}
};
template <typename T> struct StdPixelRgba {
	typedef OrderedDither<T,std::numeric_limits<T>::max()> Dither;
	T r,g,b,a;
	inline void fillKernel(glm::vec4& colourKernel) const {
		colourKernel.r = normalize(r);
		colourKernel.g = normalize(g);
		colourKernel.b = normalize(b);
		colourKernel.a = normalize(a);
	}
	inline void fromKernel(const glm::vec4& colourKernel) {
		r = denormalize<T>(colourKernel.r);
		g = denormalize<T>(colourKernel.g);
		b = denormalize<T>(colourKernel.b);
		a = denormalize<T>(colourKernel.a);
	}
	inline void fromKernelDithered(const glm::vec4& colourKernel, const glm::ivec2& screencoord) {
		r = denormalize<T>(Dither::ditherDown(colourKernel.r,screencoord));
		g = denormalize<T>(Dither::ditherDown(colourKernel.g,screencoord));
		b = denormalize<T>(Dither::ditherDown(colourKernel.b,screencoord));
		a = denormalize<T>(colourKernel.a);
	}
};
typedef StdPixelGreyscale<uint8_t> PixelGrey8;
typedef StandardTexture<PixelGrey8> TextureGrey8;
typedef StdPixelGreyscale<uint16_t> PixelGrey16;
typedef StandardTexture<PixelGrey16> TextureGrey16;
typedef StdPixelRgb<uint8_t> PixelRgb24;
typedef StandardTexture<PixelRgb24> TextureRgb24;
typedef StdPixelRgba<uint8_t> PixelRgba32;
typedef StandardTexture<PixelRgba32> TextureRgba32;
typedef StdPixelRgb<uint16_t> PixelRgb48;
typedef StandardTexture<PixelRgb48> TextureRgb48;
typedef StdPixelRgba<uint16_t> PixelRgba64;
typedef StandardTexture<PixelRgba64> TextureRgba64;

#endif // STANDARDPIXELTYPE_HPP
