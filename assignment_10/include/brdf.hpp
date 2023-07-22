#ifndef ASSIGNMENT_10_INCLUDE_BRDF_HPP
#define ASSIGNMENT_10_INCLUDE_BRDF_HPP
#include "frame.hpp"
#include "hit.h"
#include "vectors.h"
class BRDF {
   public:
    BRDF(const Hit &hit) : frame(hit.dpdu, hit.getNormal()) {}
    Vec3f eval(const Vec3f &woWS, const Vec3f &wiWS) const {
        Vec3f wo = frame.worldToLocal(woWS);
        Vec3f wi = frame.worldToLocal(wiWS);
        return f(wo, wi);
    }
    virtual Vec3f f(const Vec3f &wo, const Vec3f &wi) const = 0;
    virtual ~BRDF() = default;
    Frame frame;

   protected:
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

   private:
    Vec3f baseColor;
    float subsurface;
    float metallic;
    float specular;
    float roughness;
};

#endif /* ASSIGNMENT_10_INCLUDE_BRDF_HPP */
