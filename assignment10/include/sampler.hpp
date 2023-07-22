#ifndef ASSIGNMENT7_SAMPLER_HPP
#define ASSIGNMENT7_SAMPLER_HPP
#include <random>

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

#endif /* ASSIGNMENT7_SAMPLER_HPP */
