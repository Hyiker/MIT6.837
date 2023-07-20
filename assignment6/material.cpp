#include "material.h"

#include <glincludes.hpp>

#include "argparser.hpp"
#include "glCanvas.h"

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
    // TODO
    return Vec3f(0, 0, 0);
}
void Noise::glSetMaterial(void) const { material1->glSetMaterial(); }
Vec3f Noise::Shade(const Ray &ray, const Hit &hit, const Vec3f &dirToLight,
                   const Vec3f &lightColor) const {
    // TODO
    return Vec3f(0, 0, 0);
}
void Marble::glSetMaterial(void) const { material1->glSetMaterial(); }
Vec3f Marble::Shade(const Ray &ray, const Hit &hit, const Vec3f &dirToLight,
                    const Vec3f &lightColor) const {
    // TODO
    return Vec3f(0, 0, 0);
}