#ifndef ASSIGNMENT_10_INCLUDE_LIGHT_HPP
#define ASSIGNMENT_10_INCLUDE_LIGHT_HPP

#include "object3d.h"
#include "vectors.h"

// ====================================================================
// ====================================================================
class Light;
class PBREmitter;
struct DirectSamplingRecord;
class Light {
   public:
    // CONSTRUCTOR & DESTRUCTOR
    Light() {}
    virtual ~Light() {}

    // VIRTUAL METHODS
    // p -> light
    virtual void getIllumination(const Vec3f &p, Vec3f &dir, Vec3f &col,
                                 float &distanceToLight) const = 0;
    virtual void glInit(int id) = 0;
};

struct DirectSamplingRecord {
    // surface ref point
    Vec3f ref;
    // light sampling result point
    Vec3f point;
    const PBREmitter *emitter;

    bool isOccluded(float tShadowRay) const {
        return (point - ref).Length() > tShadowRay;
    }
};

class PBREmitter {
   public:
    PBREmitter() {}
    virtual ~PBREmitter() {}
    // accept the surface point, returning wi, pdf, and radiance
    virtual Vec3f sampleLi(const Vec3f &p, float *pdf,
                           DirectSamplingRecord *record) const {
        return Vec3f(0, 0, 0);
    }
    virtual float pdfLi(const DirectSamplingRecord &record) const { return 0; }
    // virtual Vec3f getRadiance() const = 0;
    // virtual Vec3f getRadiance(const Vec3f &p, const Vec3f &n,
    //                           const Vec3f &w) const = 0;
    // virtual Vec3f sample(const Vec3f &p, const Vec3f &n, Vec3f &wi,
    //                      float &pdf) const = 0;
    // virtual float pdf(const Vec3f &p, const Vec3f &n,
    //                   const Vec3f &wi) const = 0;
};
// ====================================================================
// ====================================================================

class DirectionalLight : public Light {
   public:
    // CONSTRUCTOR & DESTRUCTOR
    DirectionalLight(const Vec3f &d, const Vec3f &c) {
        direction = d;
        direction.Normalize();
        color = c;
    }
    ~DirectionalLight() {}

    // VIRTUAL METHODS
    void getIllumination(const Vec3f &p, Vec3f &dir, Vec3f &col,
                         float &distanceToLight) const {
        // the direction to the light is the opposite of the
        // direction of the directional light source
        dir = direction * (-1.0f);
        col = color;
        // the distance is INFINITY
        distanceToLight = INFINITY;
    }

    void glInit(int id);

   private:
    DirectionalLight();  // don't use

    // REPRESENTATION
    Vec3f direction;
    Vec3f color;
};

// ====================================================================
// ====================================================================

class PointLight : public Light {
   public:
    // CONSTRUCTOR & DESTRUCTOR
    PointLight(const Vec3f &p, const Vec3f &c, float a1, float a2, float a3) {
        position = p;
        color = c;
        attenuation_1 = a1;
        attenuation_2 = a2;
        attenuation_3 = a3;
    }
    ~PointLight() {}

    // VIRTUAL METHODS
    void getIllumination(const Vec3f &p, Vec3f &dir, Vec3f &col,
                         float &distanceToLight) const {
        dir = position - p;
        // grab the length before the direction is normalized
        distanceToLight = dir.Length();
        dir.Normalize();
        float attenuation =
            1 / (attenuation_1 + attenuation_2 * distanceToLight +
                 attenuation_3 * distanceToLight * distanceToLight);
        if (attenuation < 0) attenuation = 0;
        col = color * attenuation;
    }

    void glInit(int id);

   private:
    PointLight();  // don't use

    // REPRESENTATION
    Vec3f position;
    Vec3f color;

    // attenuation of the light
    float attenuation_1;
    float attenuation_2;
    float attenuation_3;
};

// ====================================================================
// ====================================================================

#endif /* ASSIGNMENT_10_INCLUDE_LIGHT_HPP */
