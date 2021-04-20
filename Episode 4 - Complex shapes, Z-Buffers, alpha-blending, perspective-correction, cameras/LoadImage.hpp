#ifndef LOADIMAGE_HPP
#define LOADIMAGE_HPP
#include "Texture.hpp"
#include <memory>
#include <string>

std::shared_ptr<Texture> loadImage(const char* path);
inline std::shared_ptr<Texture> loadImage(const std::string& path) {
	return loadImage(path.c_str());
}

#endif // LOADIMAGE_HPP
