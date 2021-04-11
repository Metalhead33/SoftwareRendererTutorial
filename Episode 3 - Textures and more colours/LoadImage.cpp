#include "LoadImage.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "StandardPixelType.hpp"

std::shared_ptr<Texture> loadImage(const char *path)
{
	int width, height, channels;
	unsigned char *img = stbi_load(path, &width, &height, &channels, 0);
	std::shared_ptr<Texture> toReturn = nullptr;
	if(!img) return toReturn;
	switch (channels) {
	case 1:
		toReturn = std::shared_ptr<Texture>(new TextureGrey8(reinterpret_cast<PixelGrey8*>(img),width,height));
		break;
	case 3:
		toReturn = std::shared_ptr<Texture>(new TextureRgb24(reinterpret_cast<PixelRgb24*>(img),width,height));
		break;
	case 4:
		toReturn = std::shared_ptr<Texture>(new TextureRgba32(reinterpret_cast<PixelRgba32*>(img),width,height));
		break;
	default:
		break;
	}
	stbi_image_free(img);
	return toReturn;
}
