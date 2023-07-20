#ifndef ASSIGNMENT5_TRANSFORM_HPP
#define ASSIGNMENT5_TRANSFORM_HPP
#include "matrix.h"
#include "object3d.h"
class Transform : public Object3D {
   public:
    Transform(Matrix &m, Object3D *obj)
        : m_matrix(m), m_matrix_inv(m), m_object(obj) {
        m_matrix_inv.Inverse();
        if (obj && obj->getBoundingBox()) {
            // transform the 8 vertices of the bounding box
            Vec3f vertices[8];
            BoundingBox *bb = obj->getBoundingBox();
            vertices[0] =
                Vec3f(bb->getMin().x(), bb->getMin().y(), bb->getMin().z());
            vertices[1] =
                Vec3f(bb->getMin().x(), bb->getMin().y(), bb->getMax().z());
            vertices[2] =
                Vec3f(bb->getMin().x(), bb->getMax().y(), bb->getMin().z());
            vertices[3] =
                Vec3f(bb->getMin().x(), bb->getMax().y(), bb->getMax().z());
            vertices[4] =
                Vec3f(bb->getMax().x(), bb->getMin().y(), bb->getMin().z());
            vertices[5] =
                Vec3f(bb->getMax().x(), bb->getMin().y(), bb->getMax().z());
            vertices[6] =
                Vec3f(bb->getMax().x(), bb->getMax().y(), bb->getMin().z());
            vertices[7] =
                Vec3f(bb->getMax().x(), bb->getMax().y(), bb->getMax().z());
            // transform the 8 vertices
            for (int i = 0; i < 8; i++) {
                m_matrix.Transform(vertices[i]);
            }
            // get the new bounding box
            Vec3f min(INFINITY, INFINITY, INFINITY);
            Vec3f max(-INFINITY, -INFINITY, -INFINITY);
            for (int i = 0; i < 8; i++) {
                Vec3f::Min(min, vertices[i], min);
                Vec3f::Max(max, vertices[i], max);
            }
            m_bb = new BoundingBox(min, max);
        }
    }
    ~Transform() override = default;
    bool intersect(const Ray &r, Hit &h, float tmin) override;
    void paint() const override;
    void insertIntoGrid(Grid *g, Matrix *m) override;

   private:
    Matrix m_matrix, m_matrix_inv;
    Object3D *m_object;
};
#endif /* ASSIGNMENT5_TRANSFORM_HPP */
