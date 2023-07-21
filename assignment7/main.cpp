#include <algorithm>
#include <glincludes.hpp>
#include <limits>

#include "argparser.hpp"
#include "camera.h"
#include "film.h"
#include "filter.hpp"
#include "glCanvas.h"
#include "group.h"
#include "hit.h"
#include "image.h"
#include "material.h"
#include "ray.h"
#include "raytracer.hpp"
#include "raytracing_stats.h"
#include "sampler.hpp"
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
static Film* filmPtr;
static Sampler* samplerPtr;
static Filter* filterPtr;
Vec3f traceRayWithColor(float u, float v) {
    Camera* camera = scenePtr->getCamera();
    Ray ray = camera->generateRay(Vec2f(u, v));
    Hit hit;
    hit.set(numeric_limits<float>::infinity(), nullptr, Vec3f(0, 0, 0), ray);
    return rayTracerPtr->traceRay(ray, camera->getTMin(), 0, 1.0, 1, hit);
}
void traceRay(float u, float v) { traceRayWithColor(u, v); }
void renderOutputImage(const char* filename, Film* film) {
    Image color(film->getWidth(), film->getHeight());
    for (int y = 0; y < film->getHeight(); y++) {
        for (int x = 0; x < film->getWidth(); x++) {
            color.SetPixel(x, y, filterPtr->getColor(x, y, filmPtr));
        }
    }
    color.SaveTGA(filename);
}
void dispatch(const Options& opt) {
    int size_full = std::max(opt.width, opt.height);
    int x_start = (size_full - opt.width) / 2, x_end = x_start + opt.width;
    int y_start = (size_full - opt.height) / 2, y_end = y_start + opt.height;
    assert(samplerPtr != nullptr);
    // rendering full resolution, crop afterwards
    for (int y = 0; y < size_full; y++) {
        for (int x = 0; x < size_full; x++) {
            if (x < x_start || x >= x_end || y < y_start || y >= y_end)
                continue;
            int x_ = x - x_start, y_ = y - y_start;
            for (int i = 0; i < samplerPtr->getSpp(); i++) {
                // random samples [0, 1]
                Vec2f uv = samplerPtr->getSamplePosition(i);
                filmPtr->setSample(x_, y_, i, uv,
                                   traceRayWithColor((x + uv.x()) / size_full,
                                                     (y + uv.y()) / size_full));
            }
        }
    }
    if (opt.samples_file)
        filmPtr->renderSamples(const_cast<char*>(opt.samples_file),
                               opt.sample_zoom);
    if (opt.filter_file)
        filmPtr->renderFilter(const_cast<char*>(opt.filter_file),
                              opt.filter_zoom, filterPtr);
    if (opt.output_file) renderOutputImage(opt.output_file, filmPtr);
    if (opt.stats) RayTracingStats::PrintStatistics();
    printf("Done.\n");
}
void dispatchWithoutArgs() { dispatch(getOptions()); }
void initSampler(const Options& opt) {
    if (opt.uniform_samples)
        samplerPtr = new UniformSampler(opt.uniform_samples);
    else if (opt.jittered_samples)
        samplerPtr = new JitteredSampler(opt.jittered_samples);
    else if (opt.random_samples)
        samplerPtr = new RandomSampler(opt.random_samples);
    else
        // fallback to 1 spp uniform sampler: (0.5, 0.5)
        samplerPtr = new UniformSampler(1);
}
void initFilter(const Options& opt) {
    if (opt.box_filter_radius >= 0)
        filterPtr = new BoxFilter(opt.box_filter_radius);
    else if (opt.tent_filter_radius >= 0)
        filterPtr = new TentFilter(opt.tent_filter_radius);
    else if (opt.gaussian_filter_sigma >= 0)
        filterPtr = new GaussianFilter(opt.gaussian_filter_sigma);
    else
        // fallback to box filter
        filterPtr = new BoxFilter(0.01);
}
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
    // initialize sampler
    initSampler(options);
    initFilter(options);
    filmPtr = new Film(options.width, options.height, samplerPtr->getSpp());
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
        dispatch(options);
    }
    return 0;
}