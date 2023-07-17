#include "triangle.h"

bool Triangle::intersect(const Ray &r, Hit &h, float tmin) {
    // compute intersection using barycentric coordinates
    Vec3f e1 = m_v[1] - m_v[0], e2 = m_v[2] - m_v[0], p;
    Vec3f::Cross3(p, r.getDirection(), e2);
    float A = e1.Dot3(p);
    Vec3f T;
    if (A > 0) {
        T = r.getOrigin() - m_v[0];
    } else {
        T = m_v[0] - r.getOrigin();
        A = -A;
    }
    float u = T.Dot3(p);
    if (u < 0 || u > A) {
        return false;
    }
    Vec3f Q;
    Vec3f::Cross3(Q, T, e1);
    float v = r.getDirection().Dot3(Q);
    if (v < 0 || u + v > A) {
        return false;
    }
    float t = e2.Dot3(Q);
    float fInvA = 1.f / A;
    t *= fInvA;
    if (t >= tmin && t < h.getT()) {
        Vec3f normal;
        Vec3f::Cross3(normal, e1, e2);
        normal.Normalize();
        h.set(t, m_mat, normal, r);
        return true;
    }
    return false;
}