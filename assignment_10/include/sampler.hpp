#ifndef ASSIGNMENT_10_INCLUDE_SAMPLER_HPP
#define ASSIGNMENT_10_INCLUDE_SAMPLER_HPP
#include <memory>
#include <random>

#include "argparser.hpp"
#include "vectors.h"
class Sampler {
   public:
    Sampler(int spp) : m_spp(spp), m_distr(0.0f, 1.0f) {}
    virtual Vec2f getSamplePosition(int n) = 0;
    float getSample() { return m_distr(m_gen); }
    auto getSpp() { return m_spp; }

   protected:
    int m_spp;
    std::default_random_engine m_gen;
    std::uniform_real_distribution<float> m_distr;
};
class GlobalSampler : public Sampler {
   public:
    using Sampler::Sampler;
    GlobalSampler() : Sampler(1) {}
    Vec2f getSamplePosition(int) override { return Vec2f(0.f, 0.f); }

    float get1D() { return getSample(); }
    Vec2f get2D() { return Vec2f(getSample(), getSample()); }
    Vec3f get3D() { return Vec3f(getSample(), getSample(), getSample()); }
    void setSeed(int seed) { m_gen.seed(seed); }
    std::unique_ptr<GlobalSampler> clone(int seed) {
        auto sampler = std::make_unique<GlobalSampler>(m_spp);
        sampler->setSeed(seed);
        return sampler;
    }
};
class RandomSampler : public GlobalSampler {
   public:
    RandomSampler(int spp) : GlobalSampler(spp) {}
    Vec2f getSamplePosition(int n) override;
};
class UniformSampler : public GlobalSampler {
   public:
    UniformSampler(int spp) : GlobalSampler(spp), m_sqrt_spp(std::sqrt(spp)) {}
    Vec2f getSamplePosition(int n) override;

   private:
    int m_sqrt_spp = std::sqrt(m_spp);
};
class JitteredSampler : public GlobalSampler {
   public:
    JitteredSampler(int spp) : GlobalSampler(spp), m_sqrt_spp(std::sqrt(spp)) {}
    Vec2f getSamplePosition(int n) override;

   private:
    int m_sqrt_spp = std::sqrt(m_spp);
};

std::unique_ptr<Sampler> createSampler(const Options& opt);
std::unique_ptr<GlobalSampler> createGlobalSampler();

#endif /* ASSIGNMENT_10_INCLUDE_SAMPLER_HPP */
