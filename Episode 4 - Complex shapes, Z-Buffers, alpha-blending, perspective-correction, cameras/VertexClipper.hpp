#ifndef VERTEXCLIPPER_HPP
#define VERTEXCLIPPER_HPP
#include "FixedStack.hpp"
#include <glm/glm.hpp>

template <typename T> int sgn(T val)
{
	return (T(0) < val) - (val < T(0));
}

template <typename VertexType> struct VertexClipper {
	typedef FixedStack<VertexType,36> VertexStack;
	typedef FixedStack<unsigned,36> IndexStack;
	IndexStack inputList, outputList;
	float znear;
	VertexStack vertices;
	glm::vec4 getClippingEdge(int i) {
		switch (i) {
		case 0: return glm::vec4(-1, 0, 0, 1);
		case 1: return glm::vec4( 1, 0, 0, 1);
		case 2: return glm::vec4( 0,-1, 0, 1);
		case 3: return glm::vec4( 0, 1, 0, 1);
		case 4: return glm::vec4( 0, 0,-1, 1);
		case 5: return glm::vec4( 0, 0, 1, 1);
		default:
			return glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
		}
	}

	void clipToEdge(const glm::vec4& clippingEdge) {
		if(inputList.size() < 3) return;
		outputList.clear();
		unsigned idxPrev =  inputList[0];
		outputList.push_back(idxPrev);
		const VertexType& vertPrev = vertices[idxPrev];
		float dpPrev = (clippingEdge.x * vertPrev.POS.x) + (clippingEdge.y * vertPrev.POS.y) + (clippingEdge.z * vertPrev.POS.z) + (clippingEdge.w * vertPrev.POS.w);
		for(size_t j = 1; j < inputList.size(); ++j) {
			unsigned idx = inputList[j];
			const VertexType& vert = vertices[idx];
			float dp = (clippingEdge.x * vert.POS.x) + (clippingEdge.y * vert.POS.y) + (clippingEdge.z * vert.POS.z) + (clippingEdge.w * vert.POS.w);
			if (dpPrev >= 0) outputList.push_back(idxPrev);

			if (sgn(dp) != sgn(dpPrev))
			{
				float t = dp < 0 ? dpPrev / (dpPrev - dp) : -dpPrev / (dp - dpPrev);
				VertexType vOut = VertexType::interpolate(vertices[idxPrev], vertices[idx],  1.0f-t, t, false);
				vertices.push_back(vOut);
				outputList.push_back((unsigned)(vertices.size() - 1));
			}
			idxPrev = idx;
			dpPrev = dp;
		}
		inputList = outputList;
	}

	void clipTriangle(const VertexType& v0, const VertexType& v1, const VertexType& v2) {
		vertices.clear();
		vertices.push_back(v0);
		vertices.push_back(v1);
		vertices.push_back(v2);
		inputList.clear();
		outputList.clear();
		inputList.push_back(0);
		inputList.push_back(1);
		inputList.push_back(2);
		for(int i = 0; i < 6; ++i) {
			clipToEdge(getClippingEdge(i));
		}
	}
};

#endif // VERTEXCLIPPER_HPP
