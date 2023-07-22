#ifndef ASSIGNMENT10_INCLUDE_INTEGRATOR_HPP
#define ASSIGNMENT10_INCLUDE_INTEGRATOR_HPP
#include "ray.h"
#include "sampler.hpp"
#include "scene.hpp"
class Integrator {
   public:
    Integrator(int max_bounces, std::unique_ptr<GlobalSampler> sampler)
        : m_max_bounces(max_bounces), m_sampler(std::move(sampler)) {}
    virtual Vec3f L(const Scene& scene, GlobalSampler& sampler,
                    const Ray& ray) const = 0;
    virtual ~Integrator() = default;
    int getMaxBounces() const { return m_max_bounces; }
    std::unique_ptr<GlobalSampler> getSampler(int seed) const {
        return m_sampler->clone(seed);
    }

   protected:
    int m_max_bounces;
    std::unique_ptr<GlobalSampler> m_sampler;
};
class PathIntegrator : public Integrator {
   public:
    PathIntegrator(int max_bounces, std::unique_ptr<GlobalSampler> sampler)
        : Integrator(max_bounces, std::move(sampler)) {}
    Vec3f L(const Scene& scene, GlobalSampler& sampler,
            const Ray& ray) const override;
};

std::unique_ptr<Integrator> createIntegrator(const Options& opt);

#endif /* ASSIGNMENT10_INCLUDE_INTEGRATOR_HPP */
