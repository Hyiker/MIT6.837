#include "brdf.hpp"

#include <limits>

#include "sampling.hpp"

static float pow5(float v) { return (v * v) * (v * v) * v; }

template <typename T>
static T mix(T a, T b, float t) {
    return a * (1 - t) + b * t;
}

static Vec3f reflect(const Vec3f &wo, const Vec3f &n) {
    return -wo + 2 * dot(wo, n) * n;
}
static bool isSameHemisphere(const Vec3f &v1, const Vec3f &v2) {
    return v1.z() * v2.z() > 0;
}

void BRDF::sample(const Vec3f &, Vec2f u, Vec3f *wi, float *pdf) const {
    *wi = cosineWeightedHemisphereSample(u, *pdf);
}

float BRDF::pdf(const Vec3f &, const Vec3f &wi) const {
    return cosineWeightedHemiSphereSamplePdf(wi);
}

Vec3f PrincipledBRDF::f(const Vec3f &wo, const Vec3f &wi) const {
    Vec3f fDiffuse = evalDiffuse(wo, wi), fSubsurface = evalSubsurface(wo, wi);
    Vec3f diffuse = mix(fDiffuse, fSubsurface, subsurface);
    Vec3f specular = evalSpecular(wo, wi);
    Vec3f result = (1.0 - metallic) * diffuse + specular;
    return result;
}

void PrincipledBRDF::sample(const Vec3f &wo, Vec2f u, Vec3f *wi,
                            float *pdf) const {
    Vec3f h = sampleHalfVec(u);
    *wi = reflect(wo, h);
    if (!isSameHemisphere(wo, *wi)) {
        *pdf = 0;
        return;
    }
    *pdf = this->pdf(wo, *wi);
}

float PrincipledBRDF::pdf(const Vec3f &wo, const Vec3f &wi) const {
    if (!isSameHemisphere(wo, wi)) return 0;
    Vec3f h = normalize(wo + wi);
    return DsPdf(h, wi);
}

static float FD90(float cosTheta, float roughness) {
    float ret = 0.5 + 2.0 * cosTheta * cosTheta * roughness;
    return ret * ret;
}

Vec3f PrincipledBRDF::evalDiffuse(const Vec3f &wo, const Vec3f &wi) const {
    float pi = pow5(1 - cosTheta(wi));
    float po = pow5(1 - cosTheta(wo));
    Vec3f h = normalize(wo + wi);
    float cosThetaD = absDot(wi, h);
    float fd90 = FD90(cosThetaD, roughness);
    return baseColor / M_PI * (1 + (fd90 - 1.0) * pi) * (1 + (fd90 - 1.0) * po);
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
    float cosThetaD = absDot(wi, normalize(wo + wi));
    float fss90 = Fss90(cosThetaD, roughness);
    float fss = Fss(cosThetaI, cosThetaO, fss90);
    return 1.25 * baseColor / M_PI *
           (fss * (1 / (cosThetaI + cosThetaO) - 0.5) + 0.5);
}

Vec3f PrincipledBRDF::evalSpecular(const Vec3f &wo, const Vec3f &wi) const {
    Vec3f h = normalize(wo + wi);
    float cosThetaD = absDot(wi, h);
    Vec3f spec = fresnelSpecular(cosThetaD) * Ds(h) * Gs(wo, wi) /
                 (4 * cosTheta(wo) * cosTheta(wi));
    return spec;
}

Vec3f PrincipledBRDF::sampleHalfVec(Vec2f u) const {
    float alpha = roughness * roughness;
    float alphaX = alpha, alphaY = alpha;
    // no aniostropicy, random sample phi
    float phi = 2 * M_PI * u.x();
    float cPhi = cos(phi), sPhi = sin(phi);
    float phiH = cPhi * cPhi / alphaX / alphaX + sPhi * sPhi / alphaY / alphaY;
    float cTheta = std::sqrt(phiH * (1 - u.y()) / ((1 - phiH) * u.y() + phiH));
    float sTheta = std::sqrt(1 - cTheta * cTheta);
    return Vec3f(sTheta * cPhi, sTheta * sPhi, cTheta);
}
float PrincipledBRDF::DsPdf(const Vec3f &h, const Vec3f &wi) const {
    float cosThetaD = absDot(wi, h);
    return Ds(h) * cosTheta(h) / (4 * cosThetaD);
}

float PrincipledBRDF::Ds(const Vec3f &h) const {
    float alpha = roughness * roughness;
    float alphaX = alpha, alphaY = alpha;
    float denom = sinTheta2(h) * (cosPhi2(h) / alphaX / alphaX +
                                  sinPhi2(h) / alphaY / alphaY) +
                  cosTheta2(h);
    return 1 / (M_PI * alphaX * alphaY * denom * denom);
}

float PrincipledBRDF::Gs(const Vec3f &wo, const Vec3f &wi) const {
    float alpha = roughness * roughness;
    float alphaX = alpha, alphaY = alpha;
    auto lambda = [alphaX, alphaY](const Vec3f w) {
        return (-1 + std::sqrt(1.0 + (alphaX * alphaX * cosPhi2(w) +
                                      alphaY * alphaY * sinPhi2(w)) *
                                         tanTheta2(w))) /
               2.0;
    };
    float Gs1I = 1 / (1 + lambda(wo)), Gs1O = 1 / (1 + lambda(wi));
    return Gs1I * Gs1O;
}

Vec3f PrincipledBRDF::fresnelSpecular(float cosThetaD) const {
    float C0 = 0.08 * specular;
    Vec3f Cs = mix(Vec3f(C0, C0, C0), baseColor, metallic);
    return Cs + (Vec3f(1, 1, 1) - Cs) * pow5(1 - cosThetaD);
}
