#ifndef ASSIGNMENT1_OBJECT3D_HPP
#define ASSIGNMENT1_OBJECT3D_HPP
#include "hit.h"
#include "ray.h"

class Object3D {
   public:
    Object3D() = default;
    virtual ~Object3D() = default;
    virtual bool intersect(const Ray &r, Hit &h, float tmin) = 0;

   protected:
    Material *m_mat;
};
class Sphere : public Object3D {
   public:
    Sphere(const Vec3f &center, float radius, Material *material)
        : m_center(center), m_radius(radius), m_mat(material) {}
    ~Sphere() override = default;
    bool intersect(const Ray &r, Hit &h, float tmin) override;

   private:
    Vec3f m_center;
    float m_radius;
    Material *m_mat;
};
#endif /* ASSIGNMENT1_OBJECT3D_HPP */
