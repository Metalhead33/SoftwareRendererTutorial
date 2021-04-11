#ifndef NORMALIZE_HPP
#define NORMALIZE_HPP
#include <limits>
#include <cmath>
#include <type_traits>

template <typename T> struct _normalize {
	constexpr static const float minF = -1.0f * float(std::numeric_limits<T>::min());
	constexpr static const float maxF = float(std::numeric_limits<T>::max());
	constexpr static const float recMinF = ((std::numeric_limits<T>::min())) ? 1.0f / minF : 0.0f;
	constexpr static const float recMaxF = 1.0f / maxF;
	inline static constexpr float normalize(const T& val) {
		return (val < 0) ? (float(val) * recMinF) : (float(val) * recMaxF);
	}
	inline static T denormalize(const float& val) {
		return (std::signbit(val)) ? T(val*minF) : T(val*maxF);
	}
};
template <> struct _normalize<float> {
	inline constexpr static float normalize(const float& val) {
		return val;
	}
	inline constexpr static float denormalize(const float& val) {
		return val;
	}
};
template <> struct _normalize<double> {
	inline constexpr static float normalize(const double& val) {
		return float(val);
	}
	inline constexpr static double denormalize(const float& val) {
		return double(val);
	}
};

template <typename T> inline float normalize(const T& val) { return _normalize<T>::normalize(val); }
template <typename T> inline T denormalize(const float& val) { return _normalize<T>::denormalize(val); }

#endif // NORMALIZE_HPP
