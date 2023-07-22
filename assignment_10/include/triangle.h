#ifndef ASSIGNMENT6_TRIANGLE_HPP
#define ASSIGNMENT6_TRIANGLE_HPP
#include "object3d.h"
class Triangle : public Object3D {
   public:
    Triangle(const Vec3f &a, const Vec3f &b, const Vec3f &c, Material *m)
        : Object3D(m), m_v{a, b, c} {
        m_bb = new BoundingBox(
            Vec3f(min(a.x(), b.x(), c.x()), min(a.y(), b.y(), c.y()),
                  min(a.z(), b.z(), c.z())),
            Vec3f(max(a.x(), b.x(), c.x()), max(a.y(), b.y(), c.y()),
                  max(a.z(), b.z(), c.z())));
    }
    ~Triangle() override = default;
    bool intersect(const Ray &r, Hit &h, float tmin) override;
    void paint() const override;
    void insertIntoGrid(Grid *g, Matrix *m, Transform *t) override;

   private:
    Vec3f m_v[3];
};
#endif /* ASSIGNMENT6_TRIANGLE_HPP */
