#ifndef STANDARDTEXTURE_HPP
#define STANDARDTEXTURE_HPP
#include "Texture.hpp"
#include <vector>

template <typename PixelType> class StandardTexture : public Texture
{
private:
	std::vector<PixelType> buff;
	int w,h,stride;
	float fw, fh;
public:
	StandardTexture(int w, int h) : w(w), h(h), stride(w*sizeof(PixelType)), fw(w), fh(h), buff(w*h) {

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
	void* getRawPixels() { return buff.data(); }
	const void* getRawPixels() const  { return buff.data(); }
};

#endif // STANDARDTEXTURE_HPP
