#ifndef EDGEFUNCTION_HPP
#define EDGEFUNCTION_HPP
#include <glm/glm.hpp>

float edgeFunction(const glm::vec2 &a, const glm::vec2 &b, const glm::vec2 &c);
float edgeFunction(const glm::vec3 &a, const glm::vec3 &b, const glm::vec3 &c);
float edgeFunction(const glm::vec3 &a, const glm::vec3 &b, const glm::vec2 &c);
float edgeFunction(const glm::vec4 &a, const glm::vec4 &b, const glm::vec4 &c);
float edgeFunction(const glm::vec4 &a, const glm::vec4 &b, const glm::vec2 &c);
template <typename Vertex> float edgeFunction(const Vertex& v0, const Vertex& v1, const Vertex& v2) {
	return edgeFunction(v0.POS,v1.POS,v2.POS);
}


#endif // EDGEFUNCTION_HPP
