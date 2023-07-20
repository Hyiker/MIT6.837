#ifndef ASSIGNMENT5_OBJECT3D_HPP
#define ASSIGNMENT5_OBJECT3D_HPP
#include "boundingbox.h"
#include "hit.h"
#include "ray.h"
class Grid;
class Object3D {
   public:
    Object3D() = default;
    Object3D(Material *material) : m_mat(material) {}
    virtual ~Object3D() = default;
    virtual bool intersect(const Ray &r, Hit &h, float tmin) = 0;
    virtual void paint() const = 0;
    BoundingBox *getBoundingBox() const { return m_bb; }
    virtual void insertIntoGrid(Grid *, Matrix *) {}
    auto getMaterial() const { return m_mat; }

   protected:
    Material *m_mat;
    BoundingBox *m_bb{};
};
class Sphere : public Object3D {
   public:
    Sphere(const Vec3f &center, float radius, Material *material)
        : Object3D(material), m_center(center), m_radius(radius) {
        m_bb = new BoundingBox(center - Vec3f(radius, radius, radius),
                               center + Vec3f(radius, radius, radius));
    }
    ~Sphere() override = default;
    bool intersect(const Ray &r, Hit &h, float tmin) override;
    void paint() const override;
    Vec3f getPoint(float theta, float phi) const;
    Vec3f getNormal(const Vec3f &point) const;
    void insertIntoGrid(Grid *g, Matrix *m) override;

   private:
    Vec3f m_center;
    float m_radius;
};
#endif /* ASSIGNMENT5_OBJECT3D_HPP */
