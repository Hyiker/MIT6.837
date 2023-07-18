#include "plane.h"

#include <glincludes.hpp>
#include <iostream>

#include "material.h"
bool Plane::intersect(const Ray &r, Hit &h, float tmin) {
    float t =
        -(m_d + m_normal.Dot3(r.getOrigin())) / m_normal.Dot3(r.getDirection());
    if (t >= tmin && t < h.getT()) {
        h.set(t, m_mat, m_normal, r);
        return true;
    }
    return false;
}
static constexpr float BIG = 1e5;
void Plane::paint() const {
    m_mat->glSetMaterial();
    Vec3f v1 = m_normal, v2;
    // choose a vector perpendicular to v1
    if (v1.x() == 0 && v1.y() == 0) {
        v2.Set(1, 0, 0);
    } else {
        v2.Set(0, 0, 1);
    }
    Vec3f b1, b2;
    Vec3f::Cross3(b1, v1, v2);
    b1.Normalize();
    Vec3f::Cross3(b2, v1, b1);
    b2.Normalize();
    Vec3f b1neg = b1;
    b1neg.Negate();
    float offset = -m_d;
    Vec3f p1 = offset * v1 + BIG * (b1 + b2),
          p2 = offset * v1 + BIG * (b1neg + b2),
          p3 = offset * v1 + BIG * (b1neg - b2),
          p4 = offset * v1 + BIG * (b1 - b2);
    glBegin(GL_QUADS);
    glNormal3f(m_normal.x(), m_normal.y(), m_normal.z());
    glVertex3f(p1.x(), p1.y(), p1.z());
    glVertex3f(p2.x(), p2.y(), p2.z());
    glVertex3f(p3.x(), p3.y(), p3.z());
    glVertex3f(p4.x(), p4.y(), p4.z());
    glEnd();
}
