#ifndef ASSIGNMENT4_RAYTRACER_HPP
#define ASSIGNMENT4_RAYTRACER_HPP
#include "grid.h"
#include "hit.h"
#include "ray.h"
#include "scene_parser.h"

class RayTracer {
   public:
    RayTracer(SceneParser* s, int max_bounces, float cutoff_weight,
              bool shadows, Grid* grid = nullptr);
    Vec3f traceRay(Ray& ray, float tmin, int bounces, float weight,
                   float indexOfRefraction, Hit& hit) const;
    bool intersectScene(const Ray& r, Hit& h, float tmin) const;
    bool intersectSceneFast(const Ray& r, Hit& h, float tmin) const;

    ~RayTracer() = default;
    Grid* getGrid() const { return grid; }

   private:
    SceneParser* scene;
    int maxBounces;
    float cutoffWeight;
    bool shadows;
    Grid* grid;
};

#endif /* ASSIGNMENT4_RAYTRACER_HPP */
