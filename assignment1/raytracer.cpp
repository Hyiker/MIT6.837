#include <algorithm>
#include <limits>

#include "argparser.hpp"
#include "camera.h"
#include "group.h"
#include "hit.h"
#include "image.h"
#include "material.h"
#include "ray.h"
#include "scene_parser.h"
using namespace std;
void dispatch(const Options& opt, SceneParser& scene) {
    Image color(opt.width, opt.height), depth(opt.width, opt.height);
    color.SetAllPixels(scene.getBackgroundColor());
    depth.SetAllPixels(Vec3f(0, 0, 0));
    for (int y = 0; y < opt.height; y++) {
        for (int x = 0; x < opt.width; x++) {
            Vec2f p = Vec2f(x + 0.5, y + 0.5);
            p.Divide(opt.width, opt.height);
            Ray ray = scene.getCamera()->generateRay(p);
            Hit hit;
            hit.set(numeric_limits<float>::infinity(), nullptr, ray);
            if (!scene.getGroup()->intersect(ray, hit,
                                             scene.getCamera()->getTMin())) {
                continue;
            }
            Vec3f color_hit = hit.getMaterial()->getDiffuseColor();
            color.SetPixel(x, y, color_hit);
            float t = hit.getT();
            float d = (t - opt.depth_min) / (opt.depth_max - opt.depth_min);
            d = 1.0f - d;
            d = clamp(d, 0.0f, 1.0f);
            depth.SetPixel(x, y, Vec3f(d, d, d));
        }
    }
    color.SaveTGA(opt.output_file);
    depth.SaveTGA(opt.depth_file);
    printf("Done.\n");
}
int main(int argc, const char** argv) {
    Options options = parse_args(argc, argv);
    SceneParser scene_parser(options.input_file);
    dispatch(options, scene_parser);
    return 0;
}