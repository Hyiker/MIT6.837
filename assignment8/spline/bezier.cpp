#include "spline.h"
static float BezierCurveT(int i, float t) {
    assert(i >= 0 && i < 4);
    if (i == 0) {
        return pow(1 - t, 3);
    } else if (i == 1) {
        return 3 * t * pow(1 - t, 2);
    } else if (i == 2) {
        return 3 * pow(t, 2) * (1 - t);
    } else {
        return pow(t, 3);
    }
}
Vec3f BezierCurveQ(const Vec3f &v0, const Vec3f &v1, const Vec3f &v2,
                   const Vec3f &v3, float t) {
    Vec3f q;
    for (int i = 0; i < 4; i++) {
        q += BezierCurveT(i, t) * (i == 0   ? v0
                                   : i == 1 ? v1
                                   : i == 2 ? v2
                                            : v3);
    }
    return q;
}