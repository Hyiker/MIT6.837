#ifndef ASSIGNMENT1_CAMERA_HPP
#define ASSIGNMENT1_CAMERA_HPP
#include <limits>

#include "ray.h"
#include "vectors.h"
class Camera {
   public:
    Camera() = default;
    virtual ~Camera() = default;
    virtual Ray generateRay(Vec2f point) = 0;
    virtual float getTMin() const = 0;
};

class OrthographicCamera : public Camera {
   public:
    OrthographicCamera(Vec3f center, Vec3f direction, Vec3f up, float size)
        : m_center(center), m_direction(direction), m_up(up), m_size(size) {
        m_direction.Normalize();
        Vec3f::Cross3(m_horizontal, m_direction, m_up);
        m_horizontal.Normalize();
        Vec3f::Cross3(m_up, m_horizontal, m_direction);
        m_up.Normalize();
    }
    ~OrthographicCamera() override = default;
    Ray generateRay(Vec2f point) override {
        // center - (size*up)/2 - (size*horizontal)/2  ->
        // center + (size*up)/2 + (size*horizontal)/2

        Vec3f origin = m_center + (point.x() - 0.5) * m_size * m_horizontal +
                       (point.y() - 0.5) * m_size * m_up;
        return Ray(origin, m_direction);
    }
    float getTMin() const override {
        return -std::numeric_limits<float>::infinity();
    }

   private:
    Vec3f m_center;
    Vec3f m_direction;
    Vec3f m_up;
    Vec3f m_horizontal;
    float m_size;
};

#endif /* ASSIGNMENT1_CAMERA_HPP */
