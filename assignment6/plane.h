#ifndef ASSIGNMENT5_PLANE_HPP
#define ASSIGNMENT5_PLANE_HPP
#include "object3d.h"
class Plane : public Object3D {
   public:
    Plane(const Vec3f &normal, float offset, Material *m)
        : Object3D(m), m_normal(normal), m_d(-offset) {
        // no bounding box
    }
    ~Plane() override = default;
    bool intersect(const Ray &r, Hit &h, float tmin) override;
    void paint() const override;

   private:
    Vec3f m_normal;
    float m_d;
};
#endif /* ASSIGNMENT5_PLANE_HPP */
