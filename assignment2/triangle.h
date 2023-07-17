#ifndef ASSIGNMENT2_TRIANGLE_HPP
#define ASSIGNMENT2_TRIANGLE_HPP
#include "object3d.h"
class Triangle : public Object3D {
   public:
    Triangle(const Vec3f &a, const Vec3f &b, const Vec3f &c, Material *m)
        : Object3D(m), m_v{a, b, c} {}
    ~Triangle() override = default;
    bool intersect(const Ray &r, Hit &h, float tmin) override;

   private:
    Vec3f m_v[3];
};
#endif /* ASSIGNMENT2_TRIANGLE_HPP */
