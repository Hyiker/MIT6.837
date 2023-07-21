#include "filter.hpp"

#include <algorithm>

#include "sample.h"
Vec3f Filter::getColor(int i, int j, Film *film) {
    Vec3f result(0, 0, 0);
    float weightSum = 0.f;
    int supportRadius = getSupportRadius();
    for (int x = -supportRadius; x <= supportRadius; x++) {
        for (int y = -supportRadius; y <= supportRadius; y++) {
            for (int s = 0; s < film->getNumSamples(); s++) {
                int x_ = i + x, y_ = j + y;
                x_ = std::clamp(x_, 0, film->getWidth() - 1);
                y_ = std::clamp(y_, 0, film->getHeight() - 1);
                Sample sample = film->getSample(x_, y_, s);
                float weight = getWeight(sample.getPosition().x() - 0.5f + x,
                                         sample.getPosition().y() - 0.5f + y);
                result += sample.getColor() * weight;
                weightSum += weight;
            }
        }
    }
    result.Divide(weightSum, weightSum, weightSum);
    return result;
}

float BoxFilter::getWeight(float x, float y) {
    return x >= -m_radius && x <= m_radius && y >= -m_radius && y <= m_radius
               ? 1.f
               : 0.f;
}

float TentFilter::getWeight(float x, float y) {
    float d = std::sqrt(x * x + y * y);
    // move x and y to the center of the pixel
    return 1.f - d / m_radius;
}
float GaussianFilter::getWeight(float x, float y) {
    // move x and y to the center of the pixel
    float d2 = x * x + y * y;
    // if (d2 > 4 * m_sigma * m_sigma) {
    //     return 0.f;
    // }
    return exp(-d2 / (2 * m_sigma * m_sigma));
}