#include "integrator.hpp"

#include <memory>

#include "frame.hpp"
#include "material.h"
#include "sampling.hpp"
constexpr float Q = 0.15f;
Vec3f sampleLight(const Scene& scene, const Ray& ray, const Hit& hit,
                  GlobalSampler& sampler, const BRDF& brdf) {
    Vec3f L(0, 0, 0);
    // consider ambient as environment light
    // sampling hemisphere
    float lightPdf = 0;
    Vec3f wiWS = brdf.frame.localToWorld(
        cosineWeightedHemisphereSample(sampler.get2D(), lightPdf));
    Vec3f n = normalize(hit.getNormal());
    Vec3f env = scene.getAmbientLight() * absDot(n, wiWS);
    if (lightPdf > 0) {
        Vec3f f = brdf.eval(-ray.getDirection(), wiWS);
        // test visibility
        Ray shadowRay = Ray(hit.getIntersectionPoint() + n * 1e-4, wiWS);
        Hit hit;
        bool occluded = scene.intersect(shadowRay, hit, 0);
        if (!occluded) {
            L += f * env / lightPdf;
        }
    }

    return L;
}
Vec3f PathIntegrator::L(const Scene& scene, GlobalSampler& sampler,
                        const Ray& ray) const {
    Vec3f beta(1, 1, 1);
    Vec3f L(0, 0, 0);
    Ray r = ray;
    for (int bounce = 0; bounce < m_max_bounces; bounce++) {
        Hit hit;
        bool intersected = scene.intersect(r, hit, 0);
        if (!intersected) {
            if (bounce == 0) {
                L = scene.getBackgroundColor();
            }
            break;
        }

        PBRMaterial* pbrMaterial =
            dynamic_cast<PBRMaterial*>(hit.getMaterial());

        if (!pbrMaterial) {
            break;
        }

        auto brdf = pbrMaterial->getBRDF(hit);

        Vec3f woWS = -normalize(r.getDirection());
        Vec3f n = normalize(hit.getNormal());
        L += beta * sampleLight(scene, r, hit, sampler, *brdf);

        // sample brdf
        float pdf = 0;
        Vec3f wiWS = brdf->frame.localToWorld(
            cosineWeightedHemisphereSample(sampler.get2D(), pdf));
        Vec3f f = brdf->eval(woWS, wiWS);
        if (f.Length() == 0 || pdf == 0) {
            break;
        }
        beta = beta * f * absDot(n, wiWS) / pdf;

        Vec3f origin = hit.getIntersectionPoint() + 0.001 * n;
        r = Ray(origin, wiWS);

        if (bounce > 3) {
            float p = sampler.get1D();
            if (p < Q) {
                break;
            }
            beta /= 1 - Q;
        }
    }
    return L;
}
std::unique_ptr<Integrator> createIntegrator(const Options& opt) {
    std::unique_ptr<GlobalSampler> sampler = createGlobalSampler();
    return std::make_unique<PathIntegrator>(opt.max_bounces,
                                            std::move(sampler));
}
