#ifndef ASSIGNMENT_10_INCLUDE_FRAME_HPP
#define ASSIGNMENT_10_INCLUDE_FRAME_HPP
#include <algorithm>

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

inline float cosTheta(const Vec3f& w) { return w.z(); }
inline float cosTheta2(const Vec3f& w) { return w.z() * w.z(); }
inline float sinTheta2(const Vec3f& w) {
    return std::max((float)0, (float)1 - cosTheta2(w));
}
inline float sinTheta(const Vec3f& w) { return std::sqrt(sinTheta2(w)); }
inline float absCosTheta(const Vec3f& w) { return std::abs(w.z()); }
inline float tanTheta(const Vec3f& w) { return sinTheta(w) / cosTheta(w); }
inline float tanTheta2(const Vec3f& w) { return sinTheta2(w) / cosTheta2(w); }
inline float cosPhi(const Vec3f& w) {
    float st = sinTheta(w);
    return (st == 0) ? 1 : std::clamp(w.x() / st, -1.f, 1.f);
}

inline float sinPhi(const Vec3f& w) {
    float st = sinTheta(w);
    return (st == 0) ? 0 : std::clamp(w.y() / st, -1.f, 1.f);
}

inline float cosPhi2(const Vec3f& w) { return cosPhi(w) * cosPhi(w); }

inline float sinPhi2(const Vec3f& w) { return sinPhi(w) * sinPhi(w); }

#endif /* ASSIGNMENT_10_INCLUDE_FRAME_HPP */
