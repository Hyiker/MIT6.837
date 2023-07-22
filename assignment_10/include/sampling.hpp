#ifndef ASSIGNMENT10_INCLUDE_SAMPLING_HPP
#define ASSIGNMENT10_INCLUDE_SAMPLING_HPP

#include "vectors.h"
inline float uniformHemiSphereSamplePdf() { return 1.0 / (2.0 * M_PI); }
inline Vec3f uniformHemisphereSample(const Vec2f& u, float& pdf) {
    float z = u.x();
    float r = std::sqrt(std::max(0.f, 1.f - z * z));
    float phi = 2 * M_PI * u.y();
    pdf = uniformHemiSphereSamplePdf();
    return Vec3f(r * std::cos(phi), r * std::sin(phi), z);
}
inline float cosineWeightedHemiSphereSamplePdf(const Vec3f& w) {
    return w.z() / M_PI;
}
inline Vec3f cosineWeightedHemisphereSample(const Vec2f& u, float& pdf) {
    float theta = std::acos(std::sqrt(u.x()));
    float phi = 2 * M_PI * u.y();
    Vec3f w = Vec3f(std::cos(phi) * std::sin(theta),
                    std::sin(phi) * std::sin(theta), std::cos(theta));
    pdf = cosineWeightedHemiSphereSamplePdf(w);
    return w;
}

#endif /* ASSIGNMENT10_INCLUDE_SAMPLING_HPP */
