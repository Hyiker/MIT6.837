#ifndef ASSIGNMENT10_INCLUDE_INTERSECTION_HPP
#define ASSIGNMENT10_INCLUDE_INTERSECTION_HPP
#include "vectors.h"
class Frame {
   public:
    Frame(Vec3f dpdu, Vec3f n) {
        m_n = normalize(n);
        m_ss = normalize(dpdu);
        m_ts = cross(m_n, m_ss);
    }
    [[nodiscard]] Vec3f localToWorld(const Vec3f& v) const {
        return v.x() * m_ss + v.y() * m_ts + v.z() * m_n;
    }
    [[nodiscard]] Vec3f worldToLocal(const Vec3f& v) const {
        return Vec3f(dot(v, m_ss), dot(v, m_ts), dot(v, m_n));
    }

   private:
    Vec3f m_ss, m_ts, m_n;
};

#endif /* ASSIGNMENT10_INCLUDE_INTERSECTION_HPP */
