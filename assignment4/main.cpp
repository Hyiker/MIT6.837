#include <algorithm>
#include <glincludes.hpp>
#include <limits>

#include "argparser.hpp"
#include "camera.h"
#include "glCanvas.h"
#include "group.h"
#include "hit.h"
#include "image.h"
#include "light.h"
#include "material.h"
#include "ray.h"
#include "raytracer.hpp"
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
    printf("Done.\n");
}
void dispatchWithoutArgs() { dispatch(getOptions(), *scenePtr); }
int main(int argc, const char** argv) {
    glutInit(&argc, (char**)argv);
    parse_args(argc, argv);
    const Options& options = getOptions();
    scenePtr = new SceneParser(options.input_file);
    rayTracerPtr = new RayTracer(scenePtr, options.max_bounces,
                                 options.cutoff_weight, options.shadows);
    if (options.gui) {
        GLCanvas canvas;
        canvas.initialize(scenePtr, dispatchWithoutArgs, traceRay);
    } else {
        dispatch(options, *scenePtr);
    }
    return 0;
}