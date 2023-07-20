#include "raytracer.hpp"

#include "argparser.hpp"
#include "group.h"
#include "light.h"
#include "material.h"
#include "rayTree.h"
#include "raytracing_stats.h"
Vec3f reflect(const Vec3f& v, const Vec3f& n) { return v - 2 * v.Dot3(n) * n; }
bool refract(const Vec3f& wi, const Vec3f& n, float eta, Vec3f* wt) {
    // Compute $\cos \theta_\roman{t}$ using Snell's law
    float cosThetaI = n.Dot3(wi);
    float sin2ThetaI = (std::max)(0.f, 1.f - cosThetaI * cosThetaI);
    float sin2ThetaT = eta * eta * sin2ThetaI;

    // Handle total internal reflection for transmission
    if (sin2ThetaT >= 1) return false;
    float cosThetaT = std::sqrt(1 - sin2ThetaT);
    *wt = -1.0f * eta * wi + (eta * cosThetaI - cosThetaT) * Vec3f(n);
    return true;
}
RayTracer::RayTracer(SceneParser* s, int max_bounces, float cutoff_weight,
                     bool shadows, Grid* grid)
    : scene(s),
      maxBounces(max_bounces),
      cutoffWeight(cutoff_weight),
      shadows(shadows),
      grid(grid) {
    if (grid) {
        s->getGroup()->insertIntoGrid(grid, nullptr);
    }
}

bool RayTracer::intersectScene(const Ray& r, Hit& h, float tmin) const {
    return scene->getGroup()->intersect(r, h, tmin);
}
bool RayTracer::intersectSceneFast(const Ray& r, Hit& h, float tmin) const {
    return scene->getGroup()->intersect(r, h, tmin);
}
Vec3f RayTracer::traceRay(Ray& ray, float tmin, int bounces, float weight,
                          float indexOfRefraction, Hit& hit) const {
    // count non-shadow ray
    RayTracingStats::IncrementNumNonShadowRays();
    bool intersected = false;
    if (!grid)
        intersected = intersectScene(ray, hit, tmin);
    else if (!getOptions().visualize_grid)
        intersected = intersectSceneFast(ray, hit, tmin);
    else
        intersected = grid->intersect(ray, hit, tmin);

    if (bounces > maxBounces || weight < cutoffWeight) return Vec3f(0, 0, 0);
    // only contribute background color when bounce <= maxBounces
    if (!intersected) {
        return scene->getBackgroundColor();
    }
    if (bounces == 0) RayTree::SetMainSegment(ray, 0, hit.getT());
    Material* material = hit.getMaterial();

    if (material == nullptr) {
        std::cerr << "nullptr material" << std::endl;
        return Vec3f(0, 0, 0);
    }

    // add ambient to make result happy
    Vec3f color{};
    color += material->getDiffuseColor() * scene->getAmbientLight();
    Vec3f n = hit.getNormal();
    bool reverseNormal = ray.getDirection().Dot3(n) > 0;
    Vec3f n_ = reverseNormal ? -1.0f * n : n;

    Vec3f hit_p = ray.pointAtParameter(hit.getT());
    for (int i = 0; i < scene->getNumLights(); i++) {
        Light* light = scene->getLight(i);
        Vec3f wi, col;
        float distToLight;
        light->getIllumination(hit_p, wi, col, distToLight);
        // test light visibility
        if (shadows) {
            Vec3f origin = hit_p + 1e-4 * n_;
            Ray shadowRay(origin, wi);
            float tmax = distToLight;
            Hit shadowHit(tmax, nullptr, Vec3f(0, 0, 0));
            // count shadow ray
            RayTracingStats::IncrementNumShadowRays();
            bool shadowIntersected = false;
            if (!grid)
                shadowIntersected = intersectScene(shadowRay, shadowHit, 0.0);
            else if (!getOptions().visualize_grid)
                shadowIntersected =
                    intersectSceneFast(shadowRay, shadowHit, 0.0);
            else
                shadowIntersected = grid->intersect(shadowRay, shadowHit, 0.0);
            RayTree::AddShadowSegment(shadowRay, 0, shadowHit.getT());
            if (shadowIntersected) {
                continue;
            }
        }
        color += material->Shade(ray, hit, wi, col);
    }

    if (auto phongMaterial = dynamic_cast<PhongMaterial*>(material);
        phongMaterial) {
        if (phongMaterial->isReflective()) {
            Vec3f wi = reflect(ray.getDirection(), n_);
            wi.Normalize();
            // offset the ray origin to avoid self-intersection
            Vec3f origin = hit_p + 1e-4 * n_;
            Ray reflectRay(origin, wi);
            Hit reflectHit(INFINITY, nullptr, Vec3f(0, 0, 0));
            Vec3f reflectColor =
                traceRay(reflectRay, 0.0, bounces + 1,
                         weight * phongMaterial->getReflectiveColor().Length(),
                         indexOfRefraction, reflectHit);
            RayTree::AddReflectedSegment(reflectRay, 0.0, reflectHit.getT());
            color += phongMaterial->getReflectiveColor() * reflectColor;
        }
        if (phongMaterial->isTransparent()) {
            Vec3f wt;
            bool entering = ray.getDirection().Dot3(n) < 0;
            float eta = entering ? 1 / phongMaterial->getIndexOfRefraction()
                                 : phongMaterial->getIndexOfRefraction();
            bool isRefracted = refract(-1.0f * ray.getDirection(),
                                       (entering ? 1.0f : -1.0f) * n, eta, &wt);
            if (isRefracted) {
                wt.Normalize();
                Vec3f origin = hit_p + 1e-4 * n * (entering ? -1.0f : 1.0f);
                Ray refractRay(origin, wt);
                Hit refractHit(INFINITY, nullptr, Vec3f(0, 0, 0));

                Vec3f refractColor = traceRay(
                    refractRay, 0, bounces + 1,
                    weight * phongMaterial->getTransparentColor().Length(),
                    entering ? phongMaterial->getIndexOfRefraction() : 1.0,
                    refractHit);

                RayTree::AddTransmittedSegment(refractRay, 0,
                                               refractHit.getT());
                color += phongMaterial->getTransparentColor() * refractColor;
            }
        }
    }
    return color;
}