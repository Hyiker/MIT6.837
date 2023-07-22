#ifndef ASSIGNMENT7_SAMPLER_HPP
#define ASSIGNMENT7_SAMPLER_HPP
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
    GlobalSampler() : Sampler(-1) {}
    Vec2f getSamplePosition(int) override { return Vec2f(0.f, 0.f); }

    float get1D() { return getSample(); }
    Vec2f get2D() { return Vec2f(getSample(), getSample()); }
    Vec3f get3D() { return Vec3f(getSample(), getSample(), getSample()); }
    void setSeed(int seed) { m_gen.seed(seed); }
    std::unique_ptr<GlobalSampler> clone(int seed) {
        auto sampler = std::make_unique<GlobalSampler>();
        sampler->setSeed(seed);
        return sampler;
    }
};
class RandomSampler : public Sampler {
   public:
    RandomSampler(int spp) : Sampler(spp) {}
    Vec2f getSamplePosition(int n) override;
};
class UniformSampler : public Sampler {
   public:
    UniformSampler(int spp) : Sampler(spp), m_sqrt_spp(std::sqrt(spp)) {}
    Vec2f getSamplePosition(int n) override;

   private:
    int m_sqrt_spp = std::sqrt(m_spp);
};
class JitteredSampler : public Sampler {
   public:
    JitteredSampler(int spp) : Sampler(spp), m_sqrt_spp(std::sqrt(spp)) {}
    Vec2f getSamplePosition(int n) override;

   private:
    int m_sqrt_spp = std::sqrt(m_spp);
};

std::unique_ptr<Sampler> createPixelSampler(const Options& opt);
std::unique_ptr<GlobalSampler> createGlobalSampler();

#endif /* ASSIGNMENT7_SAMPLER_HPP */
