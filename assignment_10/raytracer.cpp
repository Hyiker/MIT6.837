#include "raytracer.hpp"

#include <corecrt_math.h>
#include <oneapi/tbb/blocked_range2d.h>
#include <oneapi/tbb/parallel_for.h>

#include <memory>
#include <unordered_set>

#include "argparser.hpp"
#include "camera.h"
#include "light.h"
#include "material.h"
#include "oneapi/tbb/partitioner.h"
#include "progressreporter.h"
#include "rayTree.h"
#include "raytracing_stats.h"

using oneapi::tbb::parallel_for, oneapi::tbb::blocked_range2d;
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
RayTracer::RayTracer(std::unique_ptr<Scene> scene,
                     std::unique_ptr<Integrator> integrator,
                     std::unique_ptr<Sampler> pixelSampler, float cutOffWeight)
    : m_integrator(std::move(integrator)),
      m_scene(std::move(scene)),
      m_pixel_sampler(std::move(pixelSampler)),
      cutOffWeight(cutOffWeight) {}
void RayTracer::render(Film& film) {
    assert(samplerPtr != nullptr);
    ProgressReporter reporter(
        film.getWidth() * film.getHeight() * m_pixel_sampler->getSpp(),
        "Rendering");
    constexpr int tileSize = 200;
    int nTileX = std::ceil((float)film.getWidth() / tileSize),
        nTileY = std::ceil((float)film.getHeight() / tileSize);
    int spp = m_pixel_sampler->getSpp();
    // rendering full resolution, crop afterwards
    auto renderFunc = [&](const blocked_range2d<int>& r) {
        for (int y_ = r.rows().begin(); y_ < r.rows().end(); y_++) {
            for (int x_ = r.cols().begin(); x_ < r.cols().end(); x_++) {
                int seed = x_ * film.getHeight() + y_;
                std::unique_ptr<GlobalSampler> sampler =
                    dynamic_cast<GlobalSampler&>(*m_pixel_sampler).clone(seed);
                int x0 = x_ * tileSize, x1 = (x_ + 1) * tileSize,
                    y0 = y_ * tileSize, y1 = (y_ + 1) * tileSize;
                x1 = std::min(x1, film.getWidth());
                y1 = std::min(y1, film.getHeight());
                for (int y = y0; y < y1; y++) {
                    for (int x = x0; x < x1; x++) {
                        // generate one sampler for each pixel
                        for (int i = 0; i < spp; i++) {
                            Vec2f uv = sampler->getSamplePosition(i);
                            Ray ray = generateRay(
                                Vec2f((x + uv.x()) / film.getWidth(),
                                      (y + uv.y()) / film.getHeight()));
                            film.setSample(
                                x, y, i, uv,
                                m_integrator->L(*m_scene, *sampler, ray));
                            reporter.Update();
                        }
                    }
                }
            }
        }
    };
    parallel_for(blocked_range2d<int>(0, nTileY, 0, nTileX), renderFunc,
                 tbb::auto_partitioner());

    reporter.Done();
}
Ray RayTracer::generateRay(const Vec2f& uv) const {
    Camera* camera = m_scene->getCamera();
    return camera->generateRay(uv);
}
float RayTracer::getTMin() const { return m_scene->getCamera()->getTMin(); }
Vec3f RayTracer::traceRayRadiosity(Ray& ray, float tmin, int bounces,
                                   float weight, float indexOfRefraction,
                                   Hit& hit) const {
    Options opt = getOptions();
    // count non-shadow ray
    RayTracingStats::IncrementNumNonShadowRays();
    bool intersected = false;
    intersected = m_scene->intersect(ray, hit, tmin);

    if (bounces > m_integrator->getMaxBounces() || weight < cutOffWeight)
        return Vec3f(0, 0, 0);
    // only contribute background color when bounce <= maxBounces
    if (!intersected) {
        return m_scene->getBackgroundColor();
    }
    if (bounces == 0) RayTree::SetMainSegment(ray, 0, hit.getT());
    Material* material = hit.getMaterial();

    if (material == nullptr) {
        std::cerr << "nullptr material" << std::endl;
        return Vec3f(0, 0, 0);
    }

    // add ambient to make result happy
    Vec3f color{};
    color += material->ShadeAmbient(hit.getIntersectionPoint(),
                                    m_scene->getAmbientLight());
    Vec3f n = hit.getNormal();
    bool reverseNormal = ray.getDirection().Dot3(n) > 0;
    Vec3f n_ = reverseNormal ? -1.0f * n : n;

    Vec3f hit_p = ray.pointAtParameter(hit.getT());
    for (int i = 0; i < m_scene->getNumLights(); i++) {
        Light* light = m_scene->getLight(i);
        Vec3f wi, col;
        float distToLight;
        light->getIllumination(hit_p, wi, col, distToLight);
        // test light visibility
        if (opt.shadows) {
            Vec3f origin = hit_p + 1e-4 * n_;
            Ray shadowRay(origin, wi);
            float tmax = distToLight;
            Hit shadowHit(tmax, nullptr, Vec3f(0, 0, 0));
            // count shadow ray
            RayTracingStats::IncrementNumShadowRays();
            bool shadowIntersected = false;
            shadowIntersected = m_scene->intersect(shadowRay, shadowHit, tmin);
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
            Vec3f reflectColor = traceRayRadiosity(
                reflectRay, 0.0, bounces + 1,
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

                Vec3f refractColor = traceRayRadiosity(
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