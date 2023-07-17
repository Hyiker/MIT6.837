#ifndef ASSIGNMENT2_PLANE_HPP
#define ASSIGNMENT2_PLANE_HPP
#include "object3d.h"
class Plane : public Object3D {
   public:
    Plane(const Vec3f &normal, float offset, Material *m)
        : Object3D(m), m_normal(normal), m_d(-offset) {}
    ~Plane() override = default;
    bool intersect(const Ray &r, Hit &h, float tmin) override;

   private:
    Vec3f m_normal;
    float m_d;
};
#endif /* ASSIGNMENT2_PLANE_HPP */
