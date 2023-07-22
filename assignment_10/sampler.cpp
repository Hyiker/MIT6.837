#include "sampler.hpp"
Vec2f RandomSampler::getSamplePosition(int) {
    return Vec2f(getSample(), getSample());
}
Vec2f UniformSampler::getSamplePosition(int n) {
    int x = n % m_sqrt_spp, y = n / m_sqrt_spp;
    float d = 1.f / m_sqrt_spp / 2.f;
    return Vec2f(x * d * 2.0f + d, y * d * 2.0f + d);
}
Vec2f JitteredSampler::getSamplePosition(int n) {
    float d = 1.f / m_sqrt_spp / 2.f;
    Vec2f jitter = Vec2f(getSample(), getSample());
    jitter *= 2.0f * d;
    jitter -= Vec2f(d, d);
    int x = n % m_sqrt_spp, y = n / m_sqrt_spp;
    return Vec2f(x * d * 2.0f + d + jitter.x(), y * d * 2.0f + d + jitter.y());
}

std::unique_ptr<Sampler> createPixelSampler(const Options& opt) {
    if (opt.uniform_samples)
        return std::make_unique<UniformSampler>(opt.uniform_samples);
    else if (opt.jittered_samples)
        return std::make_unique<JitteredSampler>(opt.jittered_samples);
    else if (opt.random_samples)
        return std::make_unique<RandomSampler>(opt.random_samples);
    else
        // fallback to 1 spp uniform sampler: (0.5, 0.5)
        return std::make_unique<UniformSampler>(1);
}

std::unique_ptr<GlobalSampler> createGlobalSampler() {
    return std::make_unique<GlobalSampler>();
}