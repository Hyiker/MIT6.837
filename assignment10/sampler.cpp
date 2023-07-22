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