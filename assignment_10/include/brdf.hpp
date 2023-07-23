#ifndef ASSIGNMENT_10_INCLUDE_BRDF_HPP
#define ASSIGNMENT_10_INCLUDE_BRDF_HPP
#include "frame.hpp"
#include "hit.h"
#include "vectors.h"
class BRDF {
   public:
    BRDF(const Hit &hit) : frame(hit.dpdu, hit.getNormal()) {}
    virtual ~BRDF() = default;
    Vec3f eval(const Vec3f &woWS, const Vec3f &wiWS) const {
        Vec3f wo = frame.worldToLocal(woWS);
        Vec3f wi = frame.worldToLocal(wiWS);
        return f(wo, wi);
    }
    Vec3f importanceSample(const Vec3f &woWS, Vec2f u, float *pdf) const {
        Vec3f wo = frame.worldToLocal(woWS);
        Vec3f wi;
        sample(wo, u, &wi, pdf);
        return normalize(frame.localToWorld(wi));
    }

    float importanceSamplePdf(const Vec3f &woWS, const Vec3f &wiWS) const {
        Vec3f wi = frame.worldToLocal(wiWS);
        Vec3f wo = frame.worldToLocal(woWS);
        return pdf(wo, wi);
    }

    Frame frame;

   protected:
    virtual Vec3f f(const Vec3f &wo, const Vec3f &wi) const = 0;
    virtual void sample(const Vec3f &wo, Vec2f u, Vec3f *wi, float *pdf) const;
    virtual float pdf(const Vec3f &wo, const Vec3f &wi) const;
};

class PrincipledBRDF : public BRDF {
   public:
    PrincipledBRDF(const Hit &hit, const Vec3f &baseColor, float subsurface,
                   float metallic, float specular, float roughness)
        : BRDF(hit),
          baseColor(baseColor),
          subsurface(subsurface),
          metallic(metallic),
          specular(specular),
          roughness(roughness) {}

    Vec3f f(const Vec3f &wo, const Vec3f &wi) const override;
    void sample(const Vec3f &wo, Vec2f u, Vec3f *wi, float *pdf) const override;
    float pdf(const Vec3f &wo, const Vec3f &wi) const override;

   private:
    Vec3f evalDiffuse(const Vec3f &wo, const Vec3f &wi) const;
    Vec3f evalSubsurface(const Vec3f &wo, const Vec3f &wi) const;
    Vec3f evalSpecular(const Vec3f &wo, const Vec3f &wi) const;

    Vec3f sampleHalfVec(Vec2f u) const;
    float DsPdf(const Vec3f &h, const Vec3f &wi) const;

    float Ds(const Vec3f &h) const;
    float Gs(const Vec3f &wo, const Vec3f &wi) const;

    Vec3f fresnelSpecular(float cosThetaD) const;

    Vec3f baseColor;
    float subsurface;
    float metallic;
    float specular;
    float roughness;
};

#endif /* ASSIGNMENT_10_INCLUDE_BRDF_HPP */
