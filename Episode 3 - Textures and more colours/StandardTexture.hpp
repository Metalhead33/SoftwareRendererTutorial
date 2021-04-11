#ifndef STANDARDTEXTURE_HPP
#define STANDARDTEXTURE_HPP
#include "Texture.hpp"
#include <vector>
#include <cstring>

template <typename PixelType> class StandardTexture : public Texture
{
private:
	std::vector<PixelType> buff;
	int w,h,stride;
	float fw, fh;
public:
	StandardTexture(const StandardTexture& cpy) : buff(cpy.buff), w(cpy.w), h(cpy.h), stride(cpy.stride), fw(cpy.fw), fh(cpy.fh) {
		// Copy constructor
	}
	StandardTexture(StandardTexture&& mov) : buff(std::move(mov.buff)), w(mov.w), h(mov.h), stride(mov.stride), fw(mov.fw), fh(mov.fh) {
		// Move constructor
	}
	StandardTexture& operator=(const StandardTexture& cpy) { // Copy assignment operator
		this->buff = cpy.buff;
		this->w = cpy.w;
		this->h = cpy.h;
		this->stride = cpy.stride;
		this->fw = cpy.fw;
		this->fh = cpy.fh;
		return *this;
	}
	StandardTexture& operator=(StandardTexture&& mov) { // Move assignment operator
		this->buff = std::move(mov.buff);
		this->w = mov.w;
		this->h = mov.h;
		this->stride = mov.stride;
		this->fw = mov.fw;
		this->fh = mov.fh;
		return *this;
	}
	StandardTexture(const PixelType* pixelData, int w, int h) : w(w), h(h), stride(w*sizeof(PixelType)), fw(w-1), fh(h-1), buff(w*h) {
		memcpy(buff.data(),pixelData,sizeof(PixelType) * buff.size());
	}
	StandardTexture(int w, int h) : w(w), h(h), stride(w*sizeof(PixelType)), fw(w-1), fh(h-1), buff(w*h) {
		memset(buff.data(),0,sizeof(PixelType) * buff.size());
	}
	int getWidth() const { return w; }
	float getWidthF() const { return fw; }
	int getHeight() const { return h; }
	float getHeightF() const { return fw; }
	int getStride() const { return stride; }
	// Pixel manipulation
	void getPixel(const glm::ivec2& pos, glm::vec4& colourKernel) const {
		const PixelType& pix = buff[((pos.y % h) * w) + (pos.x % w)];
		pix.fillKernel(colourKernel);
	}
	virtual void setPixel(const glm::ivec2& pos, const glm::vec4& colourKernel){
		PixelType& pix = buff[((pos.y % h) * w) + (pos.x % w)];
		pix.fromKernel(colourKernel);
	}
	virtual void setPixelDithered(const glm::ivec2& pos, const glm::ivec2& screenpos, const glm::vec4& colourKernel) {
		PixelType& pix = buff[((pos.y % h) * w) + (pos.x % w)];
		pix.fromKernelDithered(colourKernel,screenpos);
	}
	void* getRawPixels() { return buff.data(); }
	const void* getRawPixels() const  { return buff.data(); }
};

#endif // STANDARDTEXTURE_HPP
