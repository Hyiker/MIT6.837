#ifndef ASSIGNMENT3_OBJECT3D_HPP
#define ASSIGNMENT3_OBJECT3D_HPP
#include "hit.h"
#include "ray.h"

class Object3D {
   public:
    Object3D() = default;
    Object3D(Material *material) : m_mat(material) {}
    virtual ~Object3D() = default;
    virtual bool intersect(const Ray &r, Hit &h, float tmin) = 0;
    virtual void paint() const = 0;

   protected:
    Material *m_mat;
};
class Sphere : public Object3D {
   public:
    Sphere(const Vec3f &center, float radius, Material *material)
        : Object3D(material), m_center(center), m_radius(radius) {}
    ~Sphere() override = default;
    bool intersect(const Ray &r, Hit &h, float tmin) override;
    void paint() const override;
    Vec3f getPoint(float theta, float phi) const;
    Vec3f getNormal(const Vec3f &point) const;

   private:
    Vec3f m_center;
    float m_radius;
};
#endif /* ASSIGNMENT3_OBJECT3D_HPP */
