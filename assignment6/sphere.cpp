

#include <cassert>
#include <glincludes.hpp>
#include <iostream>

#include "argparser.hpp"
#include "grid.h"
#include "material.h"
#include "matrix.h"
#include "object3d.h"
#include "raytracing_stats.h"
#define ANALYTIC_SOLUTION
bool Sphere::intersect(const Ray &r, Hit &h, float tmin) {
    RayTracingStats::IncrementNumIntersections();
    Vec3f ro = r.getOrigin() - m_center;
    Vec3f rd = r.getDirection();
#ifdef ANALYTIC_SOLUTION
    float a = rd.Dot3(rd);
    float b = 2 * rd.Dot3(ro);
    float c = ro.Dot3(ro) - m_radius * m_radius;
    float delta = b * b - 4 * a * c;
    if (delta < 0) {
        return false;
    }
    float t0 = (-b - sqrt(delta)) / (2 * a), t1 = (-b + sqrt(delta)) / (2 * a);
    if (t0 < 0 && t1 < 0) {
        return false;
    }
    float t = t0 < 0 ? t1 : t0;
#else
    // geometric solution
    bool origin_inside = ro.Length() < m_radius;

    float tp = -rd.Dot3(ro);
    if (tp < 0 && !origin_inside) {
        return false;
    }
    float d2 = ro.Dot3(ro) - tp * tp;
    if (d2 > m_radius * m_radius) {
        return false;
    }

    float t_prime = sqrt(m_radius * m_radius - d2);
    float t = origin_inside ? t_prime + tp : tp - t_prime;
#endif
    if (t < tmin) {
        return false;
    }
    // scene1_07 is probably wrong, right side sphere shouldn't be visible
    // update hit iff this t is smaller than the h.t
    if (t < h.getT()) {
        Vec3f p = r.pointAtParameter(t);
        Vec3f n = p - m_center;
        n.Normalize();
        h.set(t, m_mat, n, r);
        return true;
    }
    return false;
}

void Sphere::paint() const {
    m_mat->glSetMaterial();
    const Options &options = getOptions();
    int theta_steps = options.tessellation[0],
        phi_steps = options.tessellation[1];
    bool gouruad = options.gouraud;
    // theta: [0, 360], phi: [-90, 90] (in degrees)
    float theta_step = M_PI * 2 / theta_steps, phi_step = M_PI / phi_steps;
    glBegin(GL_QUADS);
    float theta = 0, phi = -M_PI_2;
    for (int i = 0; i < theta_steps; ++i) {
        for (int j = 0; j < phi_steps; ++j) {
            Vec3f p1 = getPoint(theta, phi),
                  p2 = getPoint(theta + theta_step, phi),
                  p3 = getPoint(theta + theta_step, phi + phi_step),
                  p4 = getPoint(theta, phi + phi_step);
            Vec3f n1 = getNormal(p1), n2 = getNormal(p2), n3 = getNormal(p3),
                  n4 = getNormal(p4);
            if (gouruad) {
                glNormal3f(n1.x(), n1.y(), n1.z());
                glVertex3f(p1.x(), p1.y(), p1.z());
                glNormal3f(n2.x(), n2.y(), n2.z());
                glVertex3f(p2.x(), p2.y(), p2.z());
                glNormal3f(n3.x(), n3.y(), n3.z());
                glVertex3f(p3.x(), p3.y(), p3.z());
                glNormal3f(n4.x(), n4.y(), n4.z());
                glVertex3f(p4.x(), p4.y(), p4.z());
            } else {
                Vec3f n = (n1 + n2 + n3 + n4);
                n.Normalize();
                glNormal3f(n.x(), n.y(), n.z());
                glVertex3f(p1.x(), p1.y(), p1.z());
                glVertex3f(p2.x(), p2.y(), p2.z());
                glVertex3f(p3.x(), p3.y(), p3.z());
                glVertex3f(p4.x(), p4.y(), p4.z());
            }
            phi += phi_step;
        }
        theta += theta_step;
        phi = -M_PI_2;
    }
    glEnd();
}

Vec3f Sphere::getPoint(float theta, float phi) const {
    float x = m_radius * cos(theta) * cos(phi), y = m_radius * sin(phi),
          z = m_radius * sin(theta) * cos(phi);
    return Vec3f(x, y, z) + m_center;
}
Vec3f Sphere::getNormal(const Vec3f &point) const {
    Vec3f n = m_center - point;
    n.Normalize();
    return n;
}
void Sphere::insertIntoGrid(Grid *g, Matrix *m, Transform *t) {
    Vec3f center = m_center;
    float radius = m_radius;
    Vec3f radius3(radius, radius, radius);
    BoundingBox bb(Vec3f(INFINITY, INFINITY, INFINITY),
                   Vec3f(-INFINITY, -INFINITY, -INFINITY));
    // if sphere has a transform matrix, using bounding box to insert
    if (m != nullptr) {
        m->Transform(center);
        m->TransformDirection(radius3);
        bb = BoundingBox(center - radius3, center + radius3);
    }
    Vec3f bMin = bb.getMin(), bMax = bb.getMax();
    float half_diag = g->getVoxelSize().Length() / 2;
    for (int i = 0; i < g->Nx(); ++i) {
        for (int j = 0; j < g->Ny(); ++j) {
            for (int k = 0; k < g->Nz(); ++k) {
                if (m != nullptr) {
                    Vec3f vMin = g->getVoxelMin(i, j, k),
                          vMax = g->getVoxelMax(i, j, k);
                    if (vMin.x() <= bMax.x() && vMax.x() >= bMin.x() &&
                        vMin.y() <= bMax.y() && vMax.y() >= bMin.y() &&
                        vMin.z() <= bMax.z() && vMax.z() >= bMin.z()) {
                        g->setVoxel(i, j, k, t ? (Object3D *)t : this);
                    }
                } else {
                    Vec3f p = g->getVoxelCenter(i, j, k);
                    float d = (p - center).Length();
                    // use half_diag to detect even the slightest overlap at the
                    // corner
                    if (d <= radius + half_diag) {
                        g->setVoxel(i, j, k, t ? (Object3D *)t : this);
                    }
                }
            }
        }
    }
}
