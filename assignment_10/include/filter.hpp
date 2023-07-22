#ifndef ASSIGNMENT7_FILTER_HPP
#define ASSIGNMENT7_FILTER_HPP
#include <memory>

#include "argparser.hpp"
#include "vectors.h"
class Film;
class Filter {
   public:
    Filter() = default;
    Vec3f getColor(int i, int j, Film *film);
    virtual float getWeight(float x, float y) = 0;
    virtual int getSupportRadius() = 0;
    virtual ~Filter() = default;
};
class BoxFilter : public Filter {
   public:
    BoxFilter(float radius) : m_radius(radius) {}
    float getWeight(float x, float y) override;
    int getSupportRadius() override { return std::ceil(m_radius - 0.5); }

   private:
    float m_radius;
};

class TentFilter : public Filter {
   public:
    TentFilter(float radius) : m_radius(radius) {}
    float getWeight(float x, float y) override;
    int getSupportRadius() override { return std::ceil(m_radius - 0.5); }

   private:
    float m_radius;
};
class GaussianFilter : public Filter {
   public:
    GaussianFilter(float sigma) : m_sigma(sigma) {}
    float getWeight(float x, float y) override;
    int getSupportRadius() override { return std::ceil(2 * m_sigma - 0.5); }

   private:
    float m_sigma;
};

std::unique_ptr<Filter> createFilter(const Options &opt);
#endif /* ASSIGNMENT7_FILTER_HPP */
