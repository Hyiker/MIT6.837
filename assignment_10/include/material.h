#ifndef ASSIGNMENT_10_INCLUDE_MATERIAL_HPP
#define ASSIGNMENT_10_INCLUDE_MATERIAL_HPP

#include "brdf.hpp"
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
    virtual Vec3f ShadeAmbient(const Vec3f &, const Vec3f &lightColor) const {
        return lightColor * diffuseColor;
    }
    virtual void glSetMaterial(void) const = 0;

   protected:
    // REPRESENTATION
    Vec3f diffuseColor;
};

class PhongMaterial : public Material {
   public:
    PhongMaterial(const Vec3f &diffuseColor, const Vec3f &specularColor,
                  float exponent, const Vec3f &reflectiveColor,
                  const Vec3f &transparentColor, float indexOfRefraction)
        : Material(diffuseColor),
          specularColor(specularColor),
          exponent(exponent),
          reflectiveColor(reflectiveColor),
          transparentColor(transparentColor),
          indexOfRefraction(indexOfRefraction) {}
    ~PhongMaterial() override = default;
    Vec3f Shade(const Ray &ray, const Hit &hit, const Vec3f &dirToLight,
                const Vec3f &lightColor) const override;
    void glSetMaterial(void) const override;
    Vec3f getSpecularColor() const { return specularColor; }
    float getExponent() const { return exponent; }
    Vec3f getReflectiveColor() const { return reflectiveColor; }
    bool isReflective() const { return reflectiveColor.Length() > 0; }
    Vec3f getTransparentColor() const { return transparentColor; }
    bool isTransparent() const { return transparentColor.Length() > 0; }
    float getIndexOfRefraction() const { return indexOfRefraction; }

   private:
    Vec3f specularColor;
    float exponent;
    Vec3f reflectiveColor;
    Vec3f transparentColor;
    float indexOfRefraction;
};

class Checkerboard : public Material {
   public:
    Checkerboard(Matrix *m, Material *mat1, Material *mat2)
        : Material(Vec3f(0, 0, 0)),
          matrix(m),
          material1(mat1),
          material2(mat2) {}
    ~Checkerboard() = default;
    void glSetMaterial(void) const override;
    Vec3f Shade(const Ray &ray, const Hit &hit, const Vec3f &dirToLight,
                const Vec3f &lightColor) const override;
    Vec3f ShadeAmbient(const Vec3f &pWS,
                       const Vec3f &lightColor) const override;

   protected:
    Matrix *matrix;
    Material *material1;
    Material *material2;
};
class Noise : public Material {
   public:
    Noise(Matrix *m, Material *mat1, Material *mat2, int octaves)
        : Material(Vec3f(0, 0, 0)),
          matrix(m),
          material1(mat1),
          material2(mat2),
          octaves(octaves) {}
    ~Noise() = default;
    void glSetMaterial(void) const override;
    Vec3f Shade(const Ray &ray, const Hit &hit, const Vec3f &dirToLight,
                const Vec3f &lightColor) const override;
    Vec3f ShadeAmbient(const Vec3f &pWS,
                       const Vec3f &lightColor) const override;

   private:
    Matrix *matrix;
    Material *material1;
    Material *material2;
    int octaves;
};

class Marble : public Material {
   public:
    Marble(Matrix *m, Material *mat1, Material *mat2, int octaves,
           float frequency, float amplitude)
        : Material(Vec3f(0, 0, 0)),
          matrix(m),
          material1(mat1),
          material2(mat2),
          octaves(octaves),
          frequency(frequency),
          amplitude(amplitude) {}
    ~Marble() = default;
    void glSetMaterial(void) const override;
    Vec3f Shade(const Ray &ray, const Hit &hit, const Vec3f &dirToLight,
                const Vec3f &lightColor) const override;
    Vec3f ShadeAmbient(const Vec3f &pWS,
                       const Vec3f &lightColor) const override;

   private:
    Matrix *matrix;
    Material *material1;
    Material *material2;
    int octaves;
    float frequency;
    float amplitude;
};

using Wood = Marble;
class PBRMaterial : public Material {
   public:
    using Material::Material;
    virtual std::unique_ptr<BRDF> getBRDF(const Hit &hit) const = 0;
};
class UberMaterial : public PBRMaterial {
   public:
    UberMaterial(const Vec3f &baseColor, float subsurface, float metallic,
                 float specular, float roughness)
        : PBRMaterial(baseColor),
          baseColor(baseColor),
          subsurface(subsurface),
          metallic(metallic),
          specular(specular),
          roughness(roughness) {}

    Vec3f Shade(const Ray &ray, const Hit &hit, const Vec3f &dirToLight,
                const Vec3f &lightColor) const override;
    std::unique_ptr<BRDF> getBRDF(const Hit &hit) const override;
    void glSetMaterial(void) const override;

    Vec3f baseColor;
    float subsurface;
    float metallic;
    float specular;
    float roughness;
};

#endif /* ASSIGNMENT_10_INCLUDE_MATERIAL_HPP */
