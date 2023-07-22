#ifndef ASSIGNMENT10_INCLUDE_RAYTRACER_HPP
#define ASSIGNMENT10_INCLUDE_RAYTRACER_HPP
#include <memory>

#include "film.h"
#include "hit.h"
#include "integrator.hpp"
#include "ray.h"
#include "scene.hpp"
class RayTracer {
   public:
    RayTracer(std::unique_ptr<Scene> scene,
              std::unique_ptr<Integrator> integrator,
              std::unique_ptr<Sampler> pixel_sampler,
              float cutOffWeight = 0.01f);
    void render(Film& film);
    Ray generateRay(const Vec2f& uv) const;
    float getTMin() const;
    Vec3f traceRayRadiosity(Ray& ray, float tmin, int bounces, float weight,
                            float indexOfRefraction, Hit& hit) const;
    int getSpp() const { return m_pixel_sampler->getSpp(); }
    ~RayTracer() = default;

   private:
    std::unique_ptr<Integrator> m_integrator;
    std::unique_ptr<Scene> m_scene;
    std::unique_ptr<Sampler> m_pixel_sampler;
    float cutOffWeight;
};

#endif /* ASSIGNMENT10_INCLUDE_RAYTRACER_HPP */
