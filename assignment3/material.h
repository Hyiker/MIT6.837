#ifndef ASSIGNMENT3_MATERIAL_HPP
#define ASSIGNMENT3_MATERIAL_HPP

#include "hit.h"
#include "ray.h"
#include "vectors.h"
// You will extend this class in later assignments

class Material {
   public:
    // CONSTRUCTORS & DESTRUCTOR
    Material(const Vec3f &d_color) { diffuseColor = d_color; }
    virtual ~Material() {}

    // ACCESSORS
    virtual Vec3f getDiffuseColor() const { return diffuseColor; }
    virtual Vec3f Shade(const Ray &ray, const Hit &hit, const Vec3f &dirToLight,
                        const Vec3f &lightColor) const = 0;
    virtual void glSetMaterial(void) const = 0;

   protected:
    // REPRESENTATION
    Vec3f diffuseColor;
};

class PhongMaterial : public Material {
   public:
    PhongMaterial(const Vec3f &diffuseColor, const Vec3f &specularColor,
                  float exponent)
        : Material(diffuseColor),
          specularColor(specularColor),
          exponent(exponent) {}
    ~PhongMaterial() override = default;
    Vec3f Shade(const Ray &ray, const Hit &hit, const Vec3f &dirToLight,
                const Vec3f &lightColor) const override {
        Vec3f n = hit.getNormal();
        Vec3f wi = dirToLight;
        Vec3f wo = ray.getDirection();
        wo.Negate();
        Vec3f h = wi + wo;
        h.Normalize();
        float cosTheta = max(0.f, n.Dot3(wi));
        float NdotH = max(0.f, n.Dot3(h));
        Vec3f diffuse = cosTheta * diffuseColor;
        Vec3f specular = cosTheta > 0 ? pow(NdotH, exponent) * specularColor
                                      : Vec3f(0, 0, 0);
        return (diffuse + specular) * lightColor;
    }
    void glSetMaterial(void) const override;
    Vec3f getSpecularColor() const { return specularColor; }
    float getExponent() const { return exponent; }

   private:
    Vec3f specularColor;
    float exponent;
};

#endif /* ASSIGNMENT3_MATERIAL_HPP */
