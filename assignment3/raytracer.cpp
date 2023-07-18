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
Vec3f reflect(const Vec3f& v, const Vec3f& n) { return v - 2 * v.Dot3(n) * n; }

Vec3f shadePhongMaterial(const Ray& ray, const Hit& hit, const Options& opt,
                         SceneParser& scene, const PhongMaterial& phongMat) {
    Vec3f kd = phongMat.getDiffuseColor(), ks = phongMat.getSpecularColor();
    Vec3f hit_p = ray.pointAtParameter(hit.getT());
    Vec3f n = hit.getNormal();
    n.Normalize();
    Vec3f wo = ray.getDirection();
    wo.Negate();
    wo.Normalize();
    Vec3f ambient = kd * scene.getAmbientLight();
    Vec3f color = ambient;
    if (opt.shade_back) {
        n = faceforward(n, wo);
    } else if (!isSameHemiSphere(wo, n)) {
        return color;
    }
    for (int i = 0; i < scene.getNumLights(); i++) {
        Light* light = scene.getLight(i);
        Vec3f wi, col;
        float distToLight;
        light->getIllumination(hit_p, wi, col, distToLight);
        wi.Normalize();
        float NdotL = (std::max)(0.0f, wi.Dot3(n));
        float diffuse = NdotL;
        // Blinn-Torrance variant
        Vec3f h = wi + wo;
        h.Normalize();
        float NdotH = (std::max)(0.0f, n.Dot3(h));
        float specular = std::pow(NdotH, phongMat.getExponent());
        color += (diffuse * kd + specular * ks) * col;
    }
    return color;
}

Vec3f shadeColor(const Ray& ray, const Hit& hit, const Options& opt,
                 SceneParser& scene) {
    Material* material = hit.getMaterial();
    if (material == nullptr) {
        return scene.getBackgroundColor();
    }
    Vec3f ka = hit.getMaterial()->getDiffuseColor();
    Hit hit2(hit);
    Vec3f wo = ray.getDirection();
    wo.Negate();
    wo.Normalize();
    Vec3f ambient = ka * scene.getAmbientLight();
    Vec3f color = ambient;
    if (opt.shade_back) {
        hit2.set(hit.getT(), hit.getMaterial(),
                 faceforward(hit2.getNormal(), wo), ray);
    } else if (!isSameHemiSphere(wo, hit2.getNormal())) {
        return color;
    }
    for (int i = 0; i < scene.getNumLights(); i++) {
        Light* light = scene.getLight(i);
        Vec3f wi, col;
        float distToLight;
        light->getIllumination(hit2.getIntersectionPoint(), wi, col,
                               distToLight);
        color += material->Shade(ray, hit2, wi, col);
    }
    return color;
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
static SceneParser* scenePtr;
void dispatchWithoutArgs() { dispatch(getOptions(), *scenePtr); }

int main(int argc, const char** argv) {
    glutInit(&argc, (char**)argv);
    parse_args(argc, argv);
    const Options& options = getOptions();
    SceneParser scene_parser(options.input_file);
    scenePtr = &scene_parser;
    if (options.gui) {
        // for debug purpose, always run dispatchWithoutArgs on startup
        dispatchWithoutArgs();
        GLCanvas canvas;
        canvas.initialize(&scene_parser, dispatchWithoutArgs);
    } else {
        dispatch(options, scene_parser);
    }
    return 0;
}