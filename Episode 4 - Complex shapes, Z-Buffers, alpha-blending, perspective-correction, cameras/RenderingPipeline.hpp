#ifndef RENDERINGPIPELINE_HPP
#define RENDERINGPIPELINE_HPP
#include <functional>
#include <glm/glm.hpp>
#include "Texture.hpp"
#include "EdgeFunction.hpp"
#include "VertexClipper.hpp"

template<typename VertexInType, typename VertexOutType, typename UniformType> struct RenderingPipeline {
	typedef std::function<VertexOutType(const UniformType&, const VertexInType&)> VertexShader;
	typedef std::function<void(Texture&, const glm::ivec2&, const UniformType&, const VertexOutType&)> FragmentShader;

	UniformType uniform;
	VertexShader vert;
	FragmentShader frag;
	Texture* framebuffer;
	glm::ivec4 viewport;
	bool perspectiveCorrection;
	typedef VertexClipper<VertexOutType> Clipper;
	Clipper clipper;
	Clipper::IndexStack indOut;

	void fillBottomFlatTriangle(float areaReciprocal, const VertexOutType& v0, const VertexOutType& v1, const VertexOutType& v2) {
		const float invslope1 = float(v1.POS.x - v0.POS.x) / float(v1.POS.y - v0.POS.y);
		const float invslope2 = float(v2.POS.x - v0.POS.x) / float(v2.POS.y - v0.POS.y);
		const int minY = std::clamp(int(std::trunc(v0.POS.y)),viewport[1],viewport[3]);
		const int maxY = std::clamp(int(std::trunc(v1.POS.y)),viewport[1],viewport[3]);
		for(int i = minY; i < maxY;++i) {
			const float dy = float(i) - v0.POS.y;
			const float curx1 = v0.POS.x + (invslope1 * dy);
			const float curx2 = v0.POS.x + (invslope2 * dy);
			renderScanline(areaReciprocal,i,int(std::trunc(curx1)),int(std::trunc(curx2)),v0,v1,v2);
		}
	}
	void fillTopFlatTriangle(float areaReciprocal, const VertexOutType& v0, const VertexOutType& v1, const VertexOutType& v2) {
		const float invslope1 = float(v2.POS.x - v0.POS.x) / float(v2.POS.y - v0.POS.y);
		const float invslope2 = float(v2.POS.x - v1.POS.x) / float(v2.POS.y - v1.POS.y);
		const int minY = std::clamp(int(std::trunc(v0.POS.y)),viewport[1],viewport[3]);
		const int maxY = std::clamp(int(std::trunc(v2.POS.y)),viewport[1],viewport[3]);
		for(int i = minY; i < maxY;++i) {
			const float dy = float(i) - v2.POS.y;
			const float curx1 = v2.POS.x + (invslope1 * dy);
			const float curx2 = v2.POS.x + (invslope2 * dy);
			renderScanline(areaReciprocal,i,int(std::trunc(curx1)),int(std::trunc(curx2)),v0,v1,v2);
		}
	}

	void renderScanline(float areaReciprocal, int y, int minX, int maxX, const VertexOutType& v0, const VertexOutType& v1, const VertexOutType& v2) {
		// Clamp the scanline's left and right ends into the viewport
		minX = std::max(minX,viewport[0]);
		maxX = std::min(maxX,viewport[2]);
		// Okay, let's render!
		for(int x = minX; x < maxX; ++x) {
			const glm::vec2 p = glm::vec2(float(x)+0.5f,float(y)+0.5f);
			const float w0 = edgeFunction(v1.POS, v2.POS, p) * areaReciprocal;
			const float w1 = edgeFunction(v2.POS, v0.POS, p) * areaReciprocal;
			const float w2 = edgeFunction(v0.POS, v1.POS, p) * areaReciprocal;
			frag(*framebuffer,glm::ivec2(x,y),uniform, VertexOutType::interpolate(v0,v1,v2,w0,w1,w2,perspectiveCorrection));
		}
	}

	void rasterize(const VertexOutType& v0, const VertexOutType& v1, const VertexOutType& v2) {

		const VertexOutType *t = &v0;
		const VertexOutType *m = &v1;
		const VertexOutType *b = &v2;

		// Sort by Y
		if (t->POS.y > m->POS.y) std::swap(t, m);
		if (m->POS.y > b->POS.y) std::swap(m, b);
		if (t->POS.y > m->POS.y) std::swap(t, m);
		const float dy = (b->POS.y - t->POS.y);
		const float iy = (m->POS.y - t->POS.y);
		if (m->POS.y == t->POS.y)
		{
			const VertexOutType *l = m, *r = t;
			if (l->POS.x > r->POS.x) std::swap(l, r);
			const float area = edgeFunction(*t,*r,*b);
			const float rArea = 1.0f / area;
			fillTopFlatTriangle(rArea, *l, *r, *b);
		}
		else if (m->POS.y == b->POS.y)
		{
		const VertexOutType *l = m, *r = b;
			if (l->POS.x > r->POS.x) std::swap(l, r);
			const float area = edgeFunction(*t,*l,*r);
			const float rArea = 1.0f / area;
			fillBottomFlatTriangle(rArea, *t, *l, *r);
		}
		else
		  {
			// Split the triangle
			VertexOutType v4 = VertexOutType::split(*t,*m,*b,dy,iy);
			const VertexOutType *l = m, *r = &v4;
			if (l->POS.x > r->POS.x) std::swap(l, r);
			const float area1 = edgeFunction(*t,*l,*r);
			const float area2 = edgeFunction(*l,*r,*b);
			const float rArea1 = 1.0f / area1;
			const float rArea2 = 1.0f / area2;
			fillBottomFlatTriangle(rArea1, *t, *l, *r);
			fillTopFlatTriangle(rArea2, *l, *r, *b);
		  }
	}
	void renderTriangle(const VertexInType& i0, const VertexInType& i1, const VertexInType& i2) {
		const VertexOutType o0 = vert(uniform,i0);
		const VertexOutType o1 = vert(uniform,i1);
		const VertexOutType o2 = vert(uniform,i2);
		if( ((o0.POS.x > 1.0f || o0.POS.x < -1.0) && (o0.POS.y > 1.0f || o0.POS.y < -1.0) && (o0.POS.z > 1.0f || o0.POS.z < 0.0)) &&
				((o1.POS.x > 1.0f || o1.POS.x < -1.0) && (o1.POS.y > 1.0f || o1.POS.y < -1.0) && (o1.POS.z > 1.0f || o1.POS.z < 0.0)) &&
				((o2.POS.x > 1.0f || o2.POS.x < -1.0) && (o2.POS.y > 1.0f || o2.POS.y < -1.0) && (o2.POS.z > 1.0f || o2.POS.z < 0.0))
				) return;
		clipper.clipTriangle(o0,o1,o2);
		for(size_t i = 0; i < clipper.vertices.size(); ++i) { // Convert to screenspace.
			if(perspectiveCorrection) clipper.vertices[i].perspectiveCorrect();
			//clipper.vertices[i].POS /= clipper.vertices[i].POS.w;
			clipper.vertices[i].POS.x /= clipper.vertices[i].POS.w;
			clipper.vertices[i].POS.y /= clipper.vertices[i].POS.w;
			clipper.vertices[i].POS.z /= clipper.vertices[i].POS.w;
			//clipper.vertices[i].POS.w = clipper.vertices[i].POS.z;
			clipper.vertices[i].POS.x = (((clipper.vertices[i].POS.x + 1.0f) * 0.5f) * float(viewport[2]-viewport[0])) + float(viewport[0]);
			clipper.vertices[i].POS.y = (((clipper.vertices[i].POS.y - 1.0f) * -0.5f) * float(viewport[3]-viewport[1])) + float(viewport[1]);
		}
		indOut.clear();
		indOut.push_back(clipper.inputList[0]);
		indOut.push_back(clipper.inputList[1]);
		indOut.push_back(clipper.inputList[2]);
		for (size_t idx = 3; idx < clipper.inputList.size(); ++idx) {
			indOut.push_back(clipper.inputList[0]);
			indOut.push_back(clipper.inputList[idx - 1]);
			indOut.push_back(clipper.inputList[idx]);
		}
		for(size_t i = 0; i < indOut.size(); i+= 3) { // Finally, we got to render them
			rasterize(clipper.vertices[indOut[i]],clipper.vertices[indOut[i+1]],clipper.vertices[indOut[i+2]]);
		}
	}
	void renderTriangles(const VertexInType* vertices, size_t vertexCount) {
		for(size_t i = 0; i < vertexCount; i += 3) {
			renderTriangle( vertices[i],vertices[i+1],vertices[i+2]);
		}
	}
	void renderTriangles(const VertexInType* vertices, const unsigned* indices, size_t indexCount) {
		for(size_t i = 0; i < indexCount; i += 3) {
			renderTriangle(vertices[indices[i]],vertices[indices[i+1]],vertices[indices[i+2]]);
		}
	}
};

#endif // RENDERINGPIPELINE_HPP
