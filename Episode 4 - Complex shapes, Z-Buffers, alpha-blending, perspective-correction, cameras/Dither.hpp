#ifndef DITHER_HPP
#define DITHER_HPP
#include <cstddef>
#include <algorithm>

template <typename T, T maximum> struct OrderedDither {
	static constexpr const float MAX = ((float(maximum)+1.0f) * 8.0f) - 1.0f;
	static constexpr const float MAX_RECIPROCAL = 1.0f/float(MAX);

	static constexpr const float LookupTable[4][4] = {
		{0.0f * MAX_RECIPROCAL,  8.0f * MAX_RECIPROCAL,  -2.0f * MAX_RECIPROCAL, 10.0f * MAX_RECIPROCAL},
	   {12.0f * MAX_RECIPROCAL,  -4.0f * MAX_RECIPROCAL, 14.0f * MAX_RECIPROCAL,  -6.0f * MAX_RECIPROCAL},
		{-3.0f * MAX_RECIPROCAL, 11.0f * MAX_RECIPROCAL,  -1.0f * MAX_RECIPROCAL,  9.0f * MAX_RECIPROCAL},
	   {15.0f * MAX_RECIPROCAL,  -7.0f * MAX_RECIPROCAL, 13.0f * MAX_RECIPROCAL,  -5.0f * MAX_RECIPROCAL}
	};
	static constexpr float ditherUp(float value, const glm::ivec2& coords) {
		return std::clamp(value + LookupTable[coords.y%4][coords.x%4],0.0f,1.0f);
	}
	static constexpr float ditherDown(float value, const glm::ivec2& coords) {
		return std::clamp(value - LookupTable[coords.y%4][coords.x%4],0.0f,1.0f);
	}
};

#endif // DITHER_HPP
