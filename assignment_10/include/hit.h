#ifndef ASSIGNMENT10_INCLUDE_HIT_HPP
#define ASSIGNMENT10_INCLUDE_HIT_HPP

#include "ray.h"
#include "vectors.h"

class Material;

// ====================================================================
// ====================================================================

class Hit {
   public:
    // CONSTRUCTOR & DESTRUCTOR
    Hit() { material = NULL; }
    Hit(float _t, Material *m, Vec3f n) {
        t = _t;
        material = m;
        normal = n;
    }
    Hit(const Hit &h) {
        t = h.t;
        material = h.material;
        normal = h.normal;
        intersectionPoint = h.intersectionPoint;
    }
    ~Hit() {}

    // ACCESSORS
    float getT() const { return t; }
    Material *getMaterial() const { return material; }
    Vec3f getNormal() const { return normal; }
    Vec3f getIntersectionPoint() const { return intersectionPoint; }

    // MODIFIER
    void set(float _t, Material *m, Vec3f n, const Ray &ray) {
        t = _t;
        material = m;
        normal = n;
        intersectionPoint = ray.pointAtParameter(t);
        if (normal.Length() > 1e-4) {
            normal.Normalize();
            dpdu = Vec3f(1, 0, 0);
            if (std::abs(dot(normal, dpdu)) > 0.99) {
                dpdu = Vec3f(0, 1, 0);
            }
            dpdu = cross(normal, dpdu);
        }
    }
    Vec3f dpdu;

   private:
    // REPRESENTATION
    float t = INFINITY;
    Material *material;
    Vec3f normal;
    Vec3f intersectionPoint;
};

inline ostream &operator<<(ostream &os, const Hit &h) {
    os << "Hit <" << h.getT() << ", " << h.getNormal() << ">";
    return os;
}
// ====================================================================
// ====================================================================

#endif /* ASSIGNMENT10_INCLUDE_HIT_HPP */
