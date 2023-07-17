#include "plane.h"

#include <iostream>
bool Plane::intersect(const Ray &r, Hit &h, float tmin) {
    float t =
        -(m_d + m_normal.Dot3(r.getOrigin())) / m_normal.Dot3(r.getDirection());
    if (t >= tmin && t < h.getT()) {
        h.set(t, m_mat, m_normal, r);
        return true;
    }
    return false;
}
