#include "transform.h"

#include <glincludes.hpp>
#include <iostream>
using namespace std;
bool Transform::intersect(const Ray &r, Hit &h, float tmin) {
    // transform ray
    Vec3f origin = r.getOrigin();
    Vec3f direction = r.getDirection();
    m_matrix_inv.Transform(origin);
    m_matrix_inv.TransformDirection(direction);
    float tscale = direction.Length();
    direction.Normalize();
    Ray rayOS(origin, direction);
    // intersect
    Hit hTmp;
    hTmp.set(h.getT() * tscale, nullptr, h.getNormal(), r);
    if (m_object->intersect(rayOS, hTmp, tmin * tscale)) {
        // rescale t
        Vec3f normal = hTmp.getNormal();
        Matrix matrix_inv_transpose = m_matrix_inv;
        matrix_inv_transpose.Transpose();
        matrix_inv_transpose.TransformDirection(normal);
        normal.Normalize();
        h.set(hTmp.getT() / tscale, hTmp.getMaterial(), normal, r);
        return true;
    }
    return false;
}
void Transform::paint() const {
    glPushMatrix();
    GLfloat *glMatrix = m_matrix.glGet();
    glMultMatrixf(glMatrix);
    delete[] glMatrix;
    m_object->paint();
    glPopMatrix();
}
void Transform::insertIntoGrid(Grid *g, Matrix *m) {
    Matrix mat;
    if (m) {
        mat = *m * m_matrix;
    } else {
        mat = m_matrix;
    }
    m_object->insertIntoGrid(g, &mat);
}