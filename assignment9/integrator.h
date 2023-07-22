#ifndef ASSIGNMENT9_INTEGRATOR_HPP
#define ASSIGNMENT9_INTEGRATOR_HPP
#include "forcefield.h"
#include "particle.h"
class Integrator {
   public:
    virtual void Update(Particle* particle, ForceField* forcefield, float t,
                        float dt) = 0;
    virtual ~Integrator() = default;
    virtual Vec3f getColor() = 0;
};
class EulerIntegrator : public Integrator {
   public:
    void Update(Particle* particle, ForceField* forcefield, float t,
                float dt) override {
        Vec3f a = forcefield->getAcceleration(particle->getPosition(),
                                              particle->getMass(), t);
        Vec3f v = particle->getVelocity();
        Vec3f p = particle->getPosition();
        Vec3f p1 = p + v * dt;
        Vec3f v1 = v + a * dt;
        particle->setPosition(p1);
        particle->setVelocity(v1);
        particle->increaseAge(dt);
    }
    Vec3f getColor() override { return Vec3f(1, 0, 0); }
};
class MidpointIntegrator : public Integrator {
   public:
    void Update(Particle* particle, ForceField* forcefield, float t,
                float dt) override {
        Vec3f a_old = forcefield->getAcceleration(particle->getPosition(),
                                                  particle->getMass(), t);
        Vec3f v = particle->getVelocity();
        Vec3f p = particle->getPosition();
        Vec3f pm = p + v * dt / 2;
        Vec3f vm = v + a_old * dt / 2;
        Vec3f a_mid = forcefield->getAcceleration(pm, particle->getMass(), t);
        Vec3f p1 = p + vm * dt;
        Vec3f v1 = v + a_mid * dt;
        particle->setPosition(p1);
        particle->setVelocity(v1);
        particle->increaseAge(dt);
    }
    Vec3f getColor() override { return Vec3f(0, 1, 0); }
};

#endif /* ASSIGNMENT9_INTEGRATOR_HPP */
