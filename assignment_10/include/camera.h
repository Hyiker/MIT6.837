#ifndef ASSIGNMENT3_CAMERA_HPP
#define ASSIGNMENT3_CAMERA_HPP
#include <limits>

#include "ray.h"
#include "vectors.h"
class Camera {
   public:
    Camera() = default;
    virtual ~Camera() = default;
    virtual Ray generateRay(Vec2f point) = 0;
    virtual float getTMin() const = 0;

    virtual void glInit(int w, int h) = 0;
    virtual void glPlaceCamera(void) = 0;
    virtual void dollyCamera(float dist) = 0;
    virtual void truckCamera(float dx, float dy) = 0;
    virtual void rotateCamera(float rx, float ry) = 0;
    virtual void print() const = 0;
};

class OrthographicCamera : public Camera {
   public:
    OrthographicCamera(Vec3f center, Vec3f direction, Vec3f up, float size)
        : m_center(center), m_direction(direction), m_up(up), m_size(size) {
        m_up.Normalize();
        m_direction.Normalize();
        Vec3f::Cross3(m_horizontal, m_direction, m_up);
        m_horizontal.Normalize();
        // screen up is the normalized orthogonal up vector for the current
        // direction vector
        Vec3f::Cross3(m_screenup, m_horizontal, m_direction);
        m_screenup.Normalize();
    }
    ~OrthographicCamera() override = default;
    Ray generateRay(Vec2f point) override {
        // center - (size*up)/2 - (size*horizontal)/2  ->
        // center + (size*up)/2 + (size*horizontal)/2

        Vec3f origin = m_center + (point.x() - 0.5) * m_size * m_horizontal +
                       (point.y() - 0.5) * m_size * m_screenup;
        return Ray(origin, m_direction);
    }
    float getTMin() const override {
        return -std::numeric_limits<float>::infinity();
    }
    void glInit(int w, int h) override;
    void glPlaceCamera(void) override;
    void dollyCamera(float dist) override;
    void truckCamera(float dx, float dy) override;
    void rotateCamera(float rx, float ry) override;
    void print() const override {
        std::cout << "OrthographicCamera: " << std::endl;
        std::cout << "center: " << m_center << std::endl;
        std::cout << "direction: " << m_direction << std::endl;
        std::cout << "up: " << m_up << std::endl;
        std::cout << "size: " << m_size << std::endl;
    }

   private:
    Vec3f m_center;
    Vec3f m_direction;
    Vec3f m_up;
    Vec3f m_horizontal;
    Vec3f m_screenup;
    float m_size;
};
class PerspectiveCamera : public Camera {
   public:
    // fov in radians
    PerspectiveCamera(Vec3f center, Vec3f direction, Vec3f up, float fov)
        : m_center(center), m_direction(direction), m_up(up), m_fov(fov) {
        m_up.Normalize();
        m_direction.Normalize();
        Vec3f::Cross3(m_horizontal, m_direction, m_up);
        m_horizontal.Normalize();
        Vec3f::Cross3(m_screenup, m_horizontal, m_direction);
        m_screenup.Normalize();
    }
    ~PerspectiveCamera() override = default;
    Ray generateRay(Vec2f point) override {
        // remap point to [-0.5, 0.5]
        point -= Vec2f(0.5, 0.5);

        float distance = 0.5 / std::tan(m_fov / 2);
        Vec3f origin = m_center;
        Vec3f direction = m_direction * distance + m_horizontal * point.x() +
                          m_screenup * point.y();
        direction.Normalize();
        return Ray(origin, direction);
    }
    float getTMin() const override { return 1e-4; }
    void glInit(int w, int h) override;
    void glPlaceCamera(void) override;
    void dollyCamera(float dist) override;
    void truckCamera(float dx, float dy) override;
    void rotateCamera(float rx, float ry) override;
    void print() const override {
        std::cout << "PerspectiveCamera: " << std::endl;
        std::cout << "center: " << m_center << std::endl;
        std::cout << "direction: " << m_direction << std::endl;
        std::cout << "up: " << m_up << std::endl;
        std::cout << "fov: " << m_fov << std::endl;
    }

   private:
    Vec3f m_center;
    Vec3f m_direction;
    Vec3f m_up;
    Vec3f m_horizontal;
    Vec3f m_screenup;

    float m_fov;
};

#endif /* ASSIGNMENT3_CAMERA_HPP */
