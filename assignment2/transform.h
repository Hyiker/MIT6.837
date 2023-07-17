#ifndef ASSIGNMENT2_TRANSFORM_HPP
#define ASSIGNMENT2_TRANSFORM_HPP
#include "matrix.h"
#include "object3d.h"
class Transform : public Object3D {
   public:
    Transform(Matrix &m, Object3D *obj)
        : m_matrix(m), m_matrix_inv(m), m_object(obj) {
        m_matrix_inv.Inverse();
    }
    ~Transform() override = default;
    bool intersect(const Ray &r, Hit &h, float tmin) override;

   private:
    Matrix m_matrix, m_matrix_inv;
    Object3D *m_object;
};
#endif /* ASSIGNMENT2_TRANSFORM_HPP */
