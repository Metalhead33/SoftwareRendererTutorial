#ifndef TEXTURE_HPP
#define TEXTURE_HPP
#include <glm/glm.hpp>

class Texture
{
public:
	virtual ~Texture() = default;
	// Data getters
	virtual int getWidth() const = 0;
	virtual float getWidthF() const = 0;
	virtual int getHeight() const = 0;
	virtual float getHeightF() const = 0;
	virtual int getStride() const = 0;
	// Pixel manipulation
	virtual void getPixel(const glm::ivec2& pos, glm::vec4& colourKernel) const = 0;
	inline glm::vec4 getPixel(const glm::ivec2& pos) const {
		glm::vec4 tmp;
		getPixel(pos,tmp);
		return tmp;
	}
	virtual void setPixel(const glm::ivec2& pos, const glm::vec4& colourKernel) = 0;
	virtual void* getRawPixels() = 0;
	virtual const void* getRawPixels() const = 0;
};

#endif // TEXTURE_HPP
