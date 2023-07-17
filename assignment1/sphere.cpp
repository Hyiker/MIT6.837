#include "object3d.h"
#define ANALYTIC_SOLUTION
bool Sphere::intersect(const Ray &r, Hit &h, float tmin) {
    Vec3f ro = r.getOrigin() - m_center;
    Vec3f rd = r.getDirection();
#ifdef ANALYTIC_SOLUTION
    float a = rd.Dot3(rd);
    float b = 2 * rd.Dot3(ro);
    float c = ro.Dot3(ro) - m_radius * m_radius;
    float delta = b * b - 4 * a * c;
    if (delta < 0) {
        return false;
    }
    float t0 = (-b - sqrt(delta)) / (2 * a), t1 = (-b + sqrt(delta)) / (2 * a);
    if (t0 < 0 && t1 < 0) {
        return false;
    }
    float t = t0 < 0 ? t1 : t0;
    if (t < tmin) {
        return false;
    }
    if (t < h.getT()) {
        h.set(t, m_mat, r);
    }
#else
    // geometric solution
    bool origin_inside = ro.Length() < m_radius;

    float tp = -rd.Dot3(ro);
    if (tp < 0 && !origin_inside) {
        return false;
    }
    float d2 = ro.Dot3(ro) - tp * tp;
    if (d2 > m_radius * m_radius) {
        return false;
    }

    float t_prime = sqrt(m_radius * m_radius - d2);
    float t = origin_inside ? t_prime - tp : tp - t_prime;
    // verify t >= tmin
    if (t < tmin) {
        return false;
    }
    // update hit iff this t is smaller than the h.t
    if (t < h.getT()) {
        h.set(t, m_mat, r);
    }
#endif
    // scene1_07 is probably wrong, right side sphere shouldn't be visible
    return true;
}
