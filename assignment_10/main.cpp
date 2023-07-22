#include <algorithm>
#include <glincludes.hpp>
#include <limits>
#include <memory>

#include "argparser.hpp"
#include "film.h"
#include "filter.hpp"
#include "glCanvas.h"
#include "group.h"
#include "hit.h"
#include "material.h"
#include "ray.h"
#include "raytracer.hpp"
#include "raytracing_stats.h"
#include "scene_parser.h"
using namespace std;

bool isSameHemiSphere(const Vec3f& v1, const Vec3f& v2) {
    return v1.Dot3(v2) > 0;
}

Vec3f shadeDepth(float t, const Options& opt) {
    float d = (t - opt.depth_min) / (opt.depth_max - opt.depth_min);
    d = 1.0f - d;
    d = clamp(d, 0.0f, 1.0f);
    return Vec3f(d, d, d);
}
Vec3f shadeNormal(const Hit& hit) {
    Vec3f n = hit.getNormal();
    n.Normalize();
    return Vec3f(fabs(n.x()), fabs(n.y()), fabs(n.z()));
}

static RayTracer* rayTracerPtr;
static Film* filmPtr;
Vec3f traceRayWithColor(float u, float v) {
    Ray ray = rayTracerPtr->generateRay(Vec2f(u, v));
    Hit hit;
    hit.set(numeric_limits<float>::infinity(), nullptr, Vec3f(0, 0, 0), ray);
    return rayTracerPtr->traceRayRadiosity(
        ray, rayTracerPtr->getTMin(), getOptions().max_bounces, 1.0, 1, hit);
}
void traceRay(float u, float v) { traceRayWithColor(u, v); }
void dispatch(const Options& opt) {
    rayTracerPtr->render(*filmPtr);
    if (opt.output_file) filmPtr->saveImage(opt.output_file);
    if (opt.stats) RayTracingStats::PrintStatistics();
    printf("Done.\n");
}

void dispatchWithoutArgs() { dispatch(getOptions()); }

int main(int argc, const char** argv) {
    glutInit(&argc, (char**)argv);

    parse_args(argc, argv);

    const Options& options = getOptions();
    auto scenePtr = new SceneParser(options.input_file);
    scenePtr->getGroup()->flatten();
    Grid* gridPtr = nullptr;
    if (options.grid_size[0]) {
        assert(scenePtr->getGroup()->getBoundingBox() != nullptr);
        gridPtr = new Grid(scenePtr->getGroup()->getBoundingBox(),
                           options.grid_size[0], options.grid_size[1],
                           options.grid_size[2]);
    }
    auto integrator = createIntegrator(options);

    rayTracerPtr =
        new RayTracer(make_unique<Scene>(scenePtr, gridPtr),
                      std::move(integrator), createPixelSampler(options));
    filmPtr = new Film(options.width, options.height, rayTracerPtr->getSpp(),
                       createFilter(options));

    RayTracingStats::Initialize(
        options.width, options.height, scenePtr->getGroup()->getBoundingBox(),
        options.grid_size[0], options.grid_size[1], options.grid_size[2]);
    if (options.gui) {
        GLCanvas canvas;
        canvas.initialize(scenePtr, dispatchWithoutArgs, traceRay, gridPtr,
                          options.visualize_grid);
    } else {
        dispatch(options);
    }
    return 0;
}