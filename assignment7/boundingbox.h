#ifndef ASSIGNMENT5_BOUNDINGBOX_HPP
#define ASSIGNMENT5_BOUNDINGBOX_HPP

#include <assert.h>

#include "vectors.h"

#define min2(a, b) (((a) < (b)) ? (a) : (b))
#define max2(a, b) (((a) > (b)) ? (a) : (b))

// ====================================================================
// ====================================================================

template <class T>
inline T min(const T &a, const T &b, const T &c) {
    T answer = min2(a, b);
    return min2(answer, c);
}

template <class T>
inline T max(const T &a, const T &b, const T &c) {
    T answer = max2(a, b);
    return max2(answer, c);
}

class BoundingBox {
   public:
    // CONSTRUCTOR & DESTRUCTOR
    BoundingBox(Vec3f _min, Vec3f _max) { Set(_min, _max); }
    ~BoundingBox() {}

    // ACCESSORS
    void Get(Vec3f &_min, Vec3f &_max) const {
        _min = min;
        _max = max;
    }
    Vec3f getMin() const { return min; }
    Vec3f getMax() const { return max; }

    // MODIFIERS
    void Set(BoundingBox *bb) {
        assert(bb != NULL);
        min = bb->min;
        max = bb->max;
    }
    void Set(Vec3f _min, Vec3f _max) {
        assert(min.x() <= max.x() && min.y() <= max.y() && min.z() <= max.z());
        min = _min;
        max = _max;
    }
    void Extend(const Vec3f v) {
        min = Vec3f(min2(min.x(), v.x()), min2(min.y(), v.y()),
                    min2(min.z(), v.z()));
        max = Vec3f(max2(max.x(), v.x()), max2(max.y(), v.y()),
                    max2(max.z(), v.z()));
    }
    void Extend(BoundingBox *bb) {
        assert(bb != NULL);
        Extend(bb->min);
        Extend(bb->max);
    }

    // DEBUGGING
    void Print() const {
        printf("%f %f %f  -> %f %f %f\n", min.x(), min.y(), min.z(), max.x(),
               max.y(), max.z());
    }
    void paint() const;
    bool isInside(const Vec3f &point) const {
        return (point.x() >= min.x() && point.x() <= max.x() &&
                point.y() >= min.y() && point.y() <= max.y() &&
                point.z() >= min.z() && point.z() <= max.z());
    }

   private:
    BoundingBox() { assert(0); }  // don't use this constructor

    // REPRESENTATION
    Vec3f min;
    Vec3f max;
};

// ====================================================================
// ====================================================================

#endif /* ASSIGNMENT5_BOUNDINGBOX_HPP */
