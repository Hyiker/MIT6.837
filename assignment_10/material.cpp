#include "material.h"

#include <glincludes.hpp>
#include <memory>

#include "argparser.hpp"
#include "common.h"
#include "glCanvas.h"
#include "matrix.h"
#include "perlin_noise.h"

#ifdef SPECULAR_FIX
// OPTIONAL:  global variable allows (hacky) communication
// with glCanvas::display
extern int SPECULAR_FIX_WHICH_PASS;
#endif

Vec3f faceforward(Vec3f v, const Vec3f &ref) {
    if (v.Dot3(ref) < 0) {
        v.Negate();
    }
    return v;
}
// ====================================================================
// Set the OpenGL parameters to render with the given material
// attributes.
// ====================================================================
Vec3f PhongMaterial::Shade(const Ray &ray, const Hit &hit,
                           const Vec3f &dirToLight,
                           const Vec3f &lightColor) const {
    Vec3f n = hit.getNormal();
    Vec3f wi = dirToLight;
    Vec3f wo = ray.getDirection();
    wo.Negate();
    Vec3f h = wi + wo;
    h.Normalize();
    if (getOptions().shade_back) {
        n = faceforward(n, wo);
    }
    float cosTheta = max(0.f, n.Dot3(wi));
    float NdotH = max(0.f, n.Dot3(h));
    Vec3f diffuse = cosTheta * diffuseColor;
    Vec3f specular =
        cosTheta > 0 ? pow(NdotH, exponent) * specularColor : Vec3f(0, 0, 0);
    return (diffuse + specular) * lightColor;
}
void PhongMaterial::glSetMaterial(void) const {
    GLfloat one[4] = {1.0, 1.0, 1.0, 1.0};
    GLfloat zero[4] = {0.0, 0.0, 0.0, 0.0};
    GLfloat specular[4] = {getSpecularColor().r(), getSpecularColor().g(),
                           getSpecularColor().b(), 1.0};
    GLfloat diffuse[4] = {getDiffuseColor().r(), getDiffuseColor().g(),
                          getDiffuseColor().b(), 1.0};

    // NOTE: GL uses the Blinn Torrance version of Phong...
    float glexponent = exponent;
    if (glexponent < 0) glexponent = 0;
    if (glexponent > 128) glexponent = 128;

#if !SPECULAR_FIX

    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, &glexponent);

#else

    // OPTIONAL: 3 pass rendering to fix the specular highlight
    // artifact for small specular exponents (wide specular lobe)

    if (SPECULAR_FIX_WHICH_PASS == 0) {
        // First pass, draw only the specular highlights
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, zero);
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, zero);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, &glexponent);

    } else if (SPECULAR_FIX_WHICH_PASS == 1) {
        // Second pass, compute normal dot light
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, one);
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, zero);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, zero);
    } else {
        // Third pass, add ambient & diffuse terms
        assert(SPECULAR_FIX_WHICH_PASS == 2);
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, diffuse);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, zero);
    }

#endif
}

void Checkerboard::glSetMaterial(void) const { material1->glSetMaterial(); }
Vec3f Checkerboard::Shade(const Ray &ray, const Hit &hit,
                          const Vec3f &dirToLight,
                          const Vec3f &lightColor) const {
    Vec3f pTS = hit.getIntersectionPoint();
    matrix->Transform(pTS);
    int x = (int)floor(pTS.x()), y = (int)floor(pTS.y()),
        z = (int)floor(pTS.z());
    if ((x + y + z) % 2 == 0) {
        return material1->Shade(ray, hit, dirToLight, lightColor);
    } else {
        return material2->Shade(ray, hit, dirToLight, lightColor);
    }
}

std::unique_ptr<BRDF> Checkerboard::getBRDF(const Hit &hit) const {
    Vec3f pTS = hit.getIntersectionPoint();
    matrix->Transform(pTS);
    // FIXME: dirty trick to ignore y axis
    int x = (int)std::floor(pTS.x()), y = (int)std::floor(pTS.y()),
        z = (int)std::floor(pTS.z());
    auto pbrmat1 = dynamic_cast<PBRMaterial *>(material1),
         pbrmat2 = dynamic_cast<PBRMaterial *>(material2);
    assert(pbrmat1 != nullptr && pbrmat2 != nullptr);
    if ((x + z) % 2 == 0) {
        return pbrmat1->getBRDF(hit);
    } else {
        return pbrmat2->getBRDF(hit);
    }
}

Vec3f Checkerboard::ShadeAmbient(const Vec3f &pWS,
                                 const Vec3f &lightColor) const {
    Vec3f pTS = pWS;
    matrix->Transform(pTS);
    int x = (int)floor(pTS.x()), y = (int)floor(pTS.y()),
        z = (int)floor(pTS.z());
    if ((x + y + z) % 2 == 0) {
        return material1->ShadeAmbient(pWS, lightColor);
    } else {
        return material2->ShadeAmbient(pWS, lightColor);
    }
}

void Noise::glSetMaterial(void) const { material1->glSetMaterial(); }
static float getNoiseRange(int octaves, float amplitude) {
    double range = 0.0;
    for (int k = 0; k < octaves; k++) {
        range += sqrt(0.5) * amplitude / (1 << k);
    }
    return range;
}
Vec3f Noise::Shade(const Ray &ray, const Hit &hit, const Vec3f &dirToLight,
                   const Vec3f &lightColor) const {
    Vec3f pTS = hit.getIntersectionPoint();
    matrix->Transform(pTS);
    int p = 1;
    float noise = 0;
    for (int i = 0; i < octaves; i++) {
        Vec3f pTS2 = pTS * p;
        noise += PerlinNoise::noise(pTS2.x(), pTS2.y(), pTS2.z()) / p;
        p *= 2;
    }
    Vec3f c1 = material1->Shade(ray, hit, dirToLight, lightColor),
          c2 = material2->Shade(ray, hit, dirToLight, lightColor);
    // convert noise from [-0.707, 0.707] to [0, 1]
    float range = getNoiseRange(octaves, 1.0);
    noise = (noise + range) / (2 * range);
    Vec3f color = (noise * c1 + (1 - noise) * c2);
    // interpolate between c1 and c2 using noise
    return color;
}

Vec3f Noise::ShadeAmbient(const Vec3f &pWS, const Vec3f &lightColor) const {
    Vec3f pTS = pWS;
    matrix->Transform(pTS);
    int p = 1;
    float noise = 0;
    for (int i = 0; i < octaves; i++) {
        Vec3f pTS2 = pTS * p;
        noise += PerlinNoise::noise(pTS2.x(), pTS2.y(), pTS2.z()) / p;
        p *= 2;
    }
    Vec3f c1 = material1->ShadeAmbient(pWS, lightColor),
          c2 = material2->ShadeAmbient(pWS, lightColor);
    // convert noise from [-0.707, 0.707] to [0, 1]
    float range = getNoiseRange(octaves, 1.0);
    noise = (noise + range) / (2 * range);
    Vec3f color = (noise * c1 + (1 - noise) * c2);
    // interpolate between c1 and c2 using noise
    return color;
}

void Marble::glSetMaterial(void) const { material1->glSetMaterial(); }
Vec3f Marble::Shade(const Ray &ray, const Hit &hit, const Vec3f &dirToLight,
                    const Vec3f &lightColor) const {
    Vec3f pTS = hit.getIntersectionPoint();
    matrix->Transform(pTS);
    int p = 1;
    float noise = 0;
    for (int i = 0; i < octaves; i++) {
        Vec3f pTS2 = pTS * p;
        noise += PerlinNoise::noise(pTS2.x(), pTS2.y(), pTS2.z()) / p;
        p *= 2;
    }
    noise = std::sin(frequency * pTS.x() + amplitude * noise);
    noise = (noise + 1) / 2;
    Vec3f c1 = material1->Shade(ray, hit, dirToLight, lightColor),
          c2 = material2->Shade(ray, hit, dirToLight, lightColor);
    // interpolate between c1 and c2 using noise
    return c1 * noise + c2 * (1 - noise);
}
Vec3f Marble::ShadeAmbient(const Vec3f &pWS, const Vec3f &lightColor) const {
    Vec3f pTS = pWS;
    matrix->Transform(pTS);
    int p = 1;
    float noise = 0;
    for (int i = 0; i < octaves; i++) {
        Vec3f pTS2 = pTS * p;
        noise += PerlinNoise::noise(pTS2.x(), pTS2.y(), pTS2.z()) / p;
        p *= 2;
    }
    noise = std::sin(frequency * pTS.x() + amplitude * noise);
    noise = (noise + 1) / 2;
    Vec3f c1 = material1->ShadeAmbient(pWS, lightColor),
          c2 = material2->ShadeAmbient(pWS, lightColor);
    // interpolate between c1 and c2 using noise
    return c1 * noise + c2 * (1 - noise);
}

Vec3f UberMaterial::Shade(const Ray &ray, const Hit &hit,
                          const Vec3f &dirToLight,
                          const Vec3f &lightColor) const {
    Vec3f n = hit.getNormal();
    Vec3f wi = dirToLight;
    Vec3f wo = ray.getDirection();
    wo.Negate();
    Vec3f h = wi + wo;
    h.Normalize();
    if (getOptions().shade_back) {
        n = faceforward(n, wo);
    }
    float cosTheta = max(0.f, n.Dot3(wi));
    float NdotH = max(0.f, n.Dot3(h));
    Vec3f diffuse = cosTheta * diffuseColor;
    return (diffuse)*lightColor;
}
void UberMaterial::glSetMaterial(void) const {
    GLfloat one[4] = {1.0, 1.0, 1.0, 1.0};
    GLfloat zero[4] = {0.0, 0.0, 0.0, 0.0};
    GLfloat specular[4] = {0.0, 0.0, 0.0, 1.0};
    GLfloat diffuse[4] = {getDiffuseColor().r(), getDiffuseColor().g(),
                          getDiffuseColor().b(), 1.0};

    // NOTE: GL uses the Blinn Torrance version of Phong...
    float glexponent = 1.0;
    if (glexponent < 0) glexponent = 0;
    if (glexponent > 128) glexponent = 128;

#if !SPECULAR_FIX

    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, &glexponent);

#else

    // OPTIONAL: 3 pass rendering to fix the specular highlight
    // artifact for small specular exponents (wide specular lobe)

    if (SPECULAR_FIX_WHICH_PASS == 0) {
        // First pass, draw only the specular highlights
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, zero);
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, zero);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, &glexponent);

    } else if (SPECULAR_FIX_WHICH_PASS == 1) {
        // Second pass, compute normal dot light
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, one);
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, zero);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, zero);
    } else {
        // Third pass, add ambient & diffuse terms
        assert(SPECULAR_FIX_WHICH_PASS == 2);
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, diffuse);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, zero);
    }

#endif
}

std::unique_ptr<BRDF> UberMaterial::getBRDF(const Hit &hit) const {
    return std::make_unique<PrincipledBRDF>(hit, diffuseColor, subsurface,
                                            metallic, specular, roughness);
}