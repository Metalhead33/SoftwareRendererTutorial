#include "ZBuffer.hpp"
#include <limits>
#include <cstring>

ZBuffer::ZBuffer() : w(0), h(0)
{

}

ZBuffer::ZBuffer(unsigned w, unsigned h) : buff(w*h), w(w), h(h)
{

}

unsigned ZBuffer::getH() const
{
	return h;
}

unsigned ZBuffer::getW() const
{
	return w;
}

void ZBuffer::clear()
{
	//memset(buff.data(),0,buff.size()*sizeof(float));
	for(auto& it : buff) {
		it = std::numeric_limits<float>::infinity();
		//it = 1.0f;
	}
}

float &ZBuffer::get(unsigned x, unsigned y)
{
	return buff[(y*w)+x];
}

const float &ZBuffer::get(unsigned x, unsigned y) const
{
	return buff[(y*w)+x];
}

float *ZBuffer::operator[](unsigned y)
{
	return &buff[y*w];
}

const float *ZBuffer::operator[](unsigned y) const
{
	return &buff[y*w];
}

ZBuffer::ZBuffer(const ZBuffer &cpy) : buff(cpy.buff), w(cpy.w), h(cpy.h)
{

}

ZBuffer &ZBuffer::operator=(const ZBuffer &cpy)
{
	this->buff = cpy.buff;
	this->w = cpy.w;
	this->h = cpy.h;
	return *this;
}

ZBuffer::ZBuffer(ZBuffer &&mov) : buff(std::move(mov.buff)), w(mov.w), h(mov.h)
{
	mov.w = 0;
	mov.h = 0;
}

ZBuffer &ZBuffer::operator=(ZBuffer &&mov)
{
	this->buff = std::move(mov.buff);
	this->w = mov.w;
	this->h = mov.h;
	mov.w = 0;
	mov.h = 0;
	return *this;
}
