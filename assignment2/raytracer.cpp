#include <algorithm>
#include <limits>

#include "argparser.hpp"
#include "camera.h"
#include "group.h"
#include "hit.h"
#include "image.h"
#include "light.h"
#include "material.h"
#include "ray.h"
#include "scene_parser.h"
using namespace std;

Vec3f faceforward(Vec3f v, const Vec3f& ref) {
    if (v.Dot3(ref) < 0) {
        v.Negate();
    }
    return v;
}
bool isSameHemiSphere(const Vec3f& v1, const Vec3f& v2) {
    return v1.Dot3(v2) > 0;
}

Vec3f shadeColor(const Ray& ray, const Hit& hit, const Options& opt,
                 SceneParser& scene) {
    Vec3f kd = hit.getMaterial()->getDiffuseColor();
    Vec3f hit_p = ray.pointAtParameter(hit.getT());
    Vec3f n = hit.getNormal();
    n.Normalize();
    Vec3f wo = ray.getDirection();
    wo.Negate();
    wo.Normalize();
    if (opt.shade_back) {
        n = faceforward(n, wo);
    } else if (!isSameHemiSphere(wo, n)) {
        return Vec3f(0, 0, 0);
    }
    Vec3f ambient = kd * scene.getAmbientLight();
    Vec3f color = ambient;
    for (int i = 0; i < scene.getNumLights(); i++) {
        Light* light = scene.getLight(i);
        Vec3f wi, col;
        light->getIllumination(hit_p, wi, col);
        wi.Normalize();
        float NdotL = std::max(0.0f, wi.Dot3(n));
        Vec3f diffuse = col * NdotL;
        color += diffuse;
    }
    return color * kd;
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

void dispatch(const Options& opt, SceneParser& scene) {
    Image color(opt.width, opt.height), depth(opt.width, opt.height),
        normal(opt.width, opt.height);
    color.SetAllPixels(scene.getBackgroundColor());
    depth.SetAllPixels(Vec3f(0, 0, 0));
    normal.SetAllPixels(Vec3f(0, 0, 0));
    for (int y = 0; y < opt.height; y++) {
        for (int x = 0; x < opt.width; x++) {
            Vec2f p = Vec2f(x + 0.5, y + 0.5);
            p.Divide(opt.width, opt.height);
            Ray ray = scene.getCamera()->generateRay(p);
            Hit hit;
            hit.set(numeric_limits<float>::infinity(), nullptr, Vec3f(0, 0, 0),
                    ray);
            if (!scene.getGroup()->intersect(ray, hit,
                                             scene.getCamera()->getTMin())) {
                continue;
            }
            color.SetPixel(x, y, shadeColor(ray, hit, opt, scene));
            depth.SetPixel(x, y, shadeDepth(hit.getT(), opt));
            normal.SetPixel(x, y, shadeNormal(hit));
        }
    }
    if (opt.output_file) color.SaveTGA(opt.output_file);
    if (opt.depth_file) depth.SaveTGA(opt.depth_file);
    if (opt.normal_file) normal.SaveTGA(opt.normal_file);
    printf("Done.\n");
}
int main(int argc, const char** argv) {
    Options options = parse_args(argc, argv);
    SceneParser scene_parser(options.input_file);
    dispatch(options, scene_parser);
    return 0;
}