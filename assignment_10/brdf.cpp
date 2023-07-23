#include "brdf.hpp"

static float pow5(float v) { return (v * v) * (v * v) * v; }

Vec3f PrincipledBRDF::f(const Vec3f &wo, const Vec3f &wi) const {
    Vec3f fDiffuse = evalDiffuse(wo, wi), fSubsurface = evalSubsurface(wo, wi);
    Vec3f diffuse = (1.0 - subsurface) * fDiffuse + subsurface * fSubsurface;
    return diffuse;
}

static float FD90(float cosTheta, float roughness) {
    float ret = 0.5 + 2.0 * cosTheta * cosTheta * roughness;
    return ret * ret;
}

Vec3f PrincipledBRDF::evalDiffuse(const Vec3f &wo, const Vec3f &wi) const {
    float p = pow5(1 - cosTheta(wi));
    Vec3f h = normalize(wo + wi);
    float fd90 = FD90(cosTheta(h), roughness);
    return baseColor / M_PI * (1 + (fd90 - 1.0) * p) * (1 + (fd90 - 1.0) * p);
}

static float Fss90(float cosTheta, float roughness) {
    float ret = cosTheta * cosTheta;
    return ret * roughness;
}

static float Fss(float cosThetaI, float cosThetaO, float fss90) {
    return (1 + (fss90 - 1.0) * pow5(1 - cosThetaI)) *
           (1 + (fss90 - 1.0) * pow5(1 - cosThetaO));
}

Vec3f PrincipledBRDF::evalSubsurface(const Vec3f &wo, const Vec3f &wi) const {
    float cosThetaI = cosTheta(wi), cosThetaO = cosTheta(wo);
    float fss90 = Fss90(cosThetaI, roughness);
    float fss = Fss(cosThetaI, cosThetaO, fss90);
    return 1.25 * baseColor / M_PI *
           (fss * (1 / (cosThetaI + cosThetaO) - 0.5) + 0.5);
}