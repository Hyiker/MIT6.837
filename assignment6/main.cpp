#include <algorithm>
#include <glincludes.hpp>
#include <limits>

#include "argparser.hpp"
#include "camera.h"
#include "glCanvas.h"
#include "group.h"
#include "hit.h"
#include "image.h"
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

static SceneParser* scenePtr;
static RayTracer* rayTracerPtr;
Vec3f traceRayWithColor(float u, float v) {
    Camera* camera = scenePtr->getCamera();
    Ray ray = camera->generateRay(Vec2f(u, v));
    Hit hit;
    hit.set(numeric_limits<float>::infinity(), nullptr, Vec3f(0, 0, 0), ray);
    return rayTracerPtr->traceRay(ray, camera->getTMin(), 0, 1.0, 1, hit);
}
void traceRay(float u, float v) { traceRayWithColor(u, v); }
void dispatch(const Options& opt, SceneParser& scene) {
    Image color(opt.width, opt.height);
    color.SetAllPixels(scene.getBackgroundColor());
    for (int y = 0; y < opt.height; y++) {
        for (int x = 0; x < opt.width; x++) {
            color.SetPixel(x, y,
                           traceRayWithColor((x + 0.5) / opt.width,
                                             (y + 0.5) / opt.height));
        }
    }
    if (opt.output_file) color.SaveTGA(opt.output_file);
    if (opt.stats) RayTracingStats::PrintStatistics();
    printf("Done.\n");
}
void dispatchWithoutArgs() { dispatch(getOptions(), *scenePtr); }
int main(int argc, const char** argv) {
    glutInit(&argc, (char**)argv);
    parse_args(argc, argv);
    const Options& options = getOptions();
    scenePtr = new SceneParser(options.input_file);
    scenePtr->getGroup()->flatten();
    Grid* gridPtr = nullptr;
    if (options.grid_size[0]) {
        assert(scenePtr->getGroup()->getBoundingBox() != nullptr);
        gridPtr = new Grid(scenePtr->getGroup()->getBoundingBox(),
                           options.grid_size[0], options.grid_size[1],
                           options.grid_size[2]);
    }
    rayTracerPtr =
        new RayTracer(scenePtr, options.max_bounces, options.cutoff_weight,
                      options.shadows, gridPtr);
    RayTracingStats::Initialize(
        options.width, options.height, scenePtr->getGroup()->getBoundingBox(),
        options.grid_size[0], options.grid_size[1], options.grid_size[2]);
    if (options.visualize_grid) {
        int s = 0;
        for (int i = 0; i < gridPtr->Volume(); i++) {
            s += !gridPtr->getVoxelFlat(i).empty();
        }
        std::cout << "Total number of objects in the grid: " << s << "/"
                  << gridPtr->Volume() << std::endl;
    }
    if (options.gui) {
        GLCanvas canvas;
        canvas.initialize(scenePtr, dispatchWithoutArgs, traceRay, gridPtr,
                          options.visualize_grid);
    } else {
        dispatch(options, *scenePtr);
    }
    return 0;
}