#ifndef ASSIGNMENT10_INCLUDE_INTEGRATOR_HPP
#define ASSIGNMENT10_INCLUDE_INTEGRATOR_HPP
#include "ray.h"
#include "sampler.hpp"
#include "scene.hpp"
class Integrator {
   public:
    Integrator(int max_bounces) : m_max_bounces(max_bounces) {}
    virtual Vec3f L(const Scene& scene, GlobalSampler& sampler,
                    const Ray& ray) const = 0;
    virtual ~Integrator() = default;
    int getMaxBounces() const { return m_max_bounces; }

   protected:
    int m_max_bounces;
};
class PathIntegrator : public Integrator {
   public:
    PathIntegrator(int max_bounces) : Integrator(max_bounces) {}
    Vec3f L(const Scene& scene, GlobalSampler& sampler,
            const Ray& ray) const override;
};

std::unique_ptr<Integrator> createIntegrator(const Options& opt);

#endif /* ASSIGNMENT10_INCLUDE_INTEGRATOR_HPP */
