#ifndef ASSIGNMENT4_RAYTRACER_HPP
#define ASSIGNMENT4_RAYTRACER_HPP
#include "hit.h"
#include "ray.h"
#include "scene_parser.h"

class RayTracer {
   public:
    RayTracer(SceneParser* s, int max_bounces, float cutoff_weight,
              bool shadows)
        : scene(s),
          maxBounces(max_bounces),
          cutoffWeight(cutoff_weight),
          shadows(shadows) {}
    Vec3f traceRay(Ray& ray, float tmin, int bounces, float weight,
                   float indexOfRefraction, Hit& hit) const;
    ~RayTracer() = default;

   private:
    SceneParser* scene;
    int maxBounces;
    float cutoffWeight;
    bool shadows;
};

#endif /* ASSIGNMENT4_RAYTRACER_HPP */
