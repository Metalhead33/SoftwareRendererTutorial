#ifndef ZBUFFER_HPP
#define ZBUFFER_HPP
#include <vector>

class ZBuffer
{
private:
	std::vector<float> buff;
	unsigned w,h;
public:
	ZBuffer();
	ZBuffer(unsigned w, unsigned h);
	ZBuffer(const ZBuffer& cpy);
	ZBuffer& operator=(const ZBuffer& cpy);
	ZBuffer(ZBuffer&& mov);
	ZBuffer& operator=(ZBuffer&& mov);
	unsigned getW() const;
	unsigned getH() const;
	void clear();
	float& get(unsigned x, unsigned y);
	const float& get(unsigned x, unsigned y) const;
	float* operator[](unsigned y);
	const float* operator[](unsigned y) const;
};

#endif // ZBUFFER_HPP
