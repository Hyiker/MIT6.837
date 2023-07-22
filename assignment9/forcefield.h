#ifndef ASSIGNMENT9_FORCEFIELD_HPP
#define ASSIGNMENT9_FORCEFIELD_HPP
#include "vectors.h"
class ForceField {
   public:
    virtual Vec3f getAcceleration(const Vec3f& position, float mass,
                                  float t) const = 0;
    virtual ~ForceField() = default;
};

class GravityForceField : public ForceField {
   public:
    GravityForceField(const Vec3f& gravity) : m_gravity(gravity) {}
    Vec3f getAcceleration(const Vec3f& position, float mass,
                          float t) const override {
        Vec3f a = m_gravity;
        return a;
    }

   private:
    Vec3f m_gravity;
};

class ConstantForceField : public ForceField {
   public:
    ConstantForceField(const Vec3f& force) : m_force(force) {}
    Vec3f getAcceleration(const Vec3f& position, float mass,
                          float t) const override {
        Vec3f a = m_force / mass;
        return a;
    }

   private:
    Vec3f m_force;
};

class RadialForceField : public ForceField {
   public:
    RadialForceField(float magnitude) : m_magnitude(magnitude) {}
    Vec3f getAcceleration(const Vec3f& position, float mass,
                          float t) const override {
        Vec3f a = -m_magnitude * position;
        return a;
    }

   private:
    float m_magnitude;
};

class VerticalForceField : public ForceField {
   public:
    VerticalForceField(float magnitude) : m_magnitude(magnitude) {}
    Vec3f getAcceleration(const Vec3f& position, float mass,
                          float t) const override {
        Vec3f a = Vec3f(0, -m_magnitude, 0);
        return a;
    }

   private:
    float m_magnitude;
};

#endif /* ASSIGNMENT9_FORCEFIELD_HPP */
