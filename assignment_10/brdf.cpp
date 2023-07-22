#include "brdf.hpp"

Vec3f PrincipledBRDF::f(const Vec3f &wo, const Vec3f &wi) const {
    return baseColor;
}