#ifndef COMMON_INCLUDES_COMMON_HPP
#define COMMON_INCLUDES_COMMON_HPP
#include <algorithm>
inline float saturate(float x) { return std::clamp(x, 0.0f, 1.0f); }

#endif /* COMMON_INCLUDES_COMMON_HPP */
