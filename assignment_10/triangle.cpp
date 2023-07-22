#include "triangle.h"

#include <glincludes.hpp>

#include "grid.h"
#include "material.h"
#include "matrix.h"
#include "raytracing_stats.h"
bool Triangle::intersect(const Ray &r, Hit &h, float tmin) {
    RayTracingStats::IncrementNumIntersections();
    // compute intersection using barycentric coordinates
    Vec3f e1 = m_v[1] - m_v[0], e2 = m_v[2] - m_v[0], p;
    Vec3f::Cross3(p, r.getDirection(), e2);
    float A = e1.Dot3(p);
    Vec3f T;
    if (A > 0) {
        T = r.getOrigin() - m_v[0];
    } else {
        T = m_v[0] - r.getOrigin();
        A = -A;
    }
    float u = T.Dot3(p);
    if (u < 0 || u > A) {
        return false;
    }
    Vec3f Q;
    Vec3f::Cross3(Q, T, e1);
    float v = r.getDirection().Dot3(Q);
    if (v < 0 || u + v > A) {
        return false;
    }
    float t = e2.Dot3(Q);
    float fInvA = 1.f / A;
    t *= fInvA;
    if (t >= tmin && t <= h.getT()) {
        Vec3f normal;
        Vec3f::Cross3(normal, e1, e2);
        normal.Normalize();
        h.set(t, m_mat, normal, r);
        return true;
    }
    return false;
}
void Triangle::paint() const {
    m_mat->glSetMaterial();
    glBegin(GL_TRIANGLES);
    Vec3f e1 = m_v[1] - m_v[0], e2 = m_v[2] - m_v[0], normal;
    Vec3f::Cross3(normal, e1, e2);
    normal.Normalize();
    glNormal3f(normal.x(), normal.y(), normal.z());
    glVertex3f(m_v[0].x(), m_v[0].y(), m_v[0].z());
    glVertex3f(m_v[1].x(), m_v[1].y(), m_v[1].z());
    glVertex3f(m_v[2].x(), m_v[2].y(), m_v[2].z());
    glEnd();
}
void Triangle::insertIntoGrid(Grid *g, Matrix *m, Transform *t) {
    // for now, simply test if the triangle's bounding box intersects the grid
    Vec3f bMin = m_bb->getMin(), bMax = m_bb->getMax();
    if (m) {
        // compute the transformed bounding box
        Vec3f v[3]{m_v[0], m_v[1], m_v[2]};
        for (int i = 0; i < 3; i++) {
            m->Transform(v[i]);
        }
        bMin = bMax = v[0];
        Vec3f::Min(bMin, bMin, v[1]);
        Vec3f::Min(bMin, bMin, v[2]);
        Vec3f::Max(bMax, bMax, v[1]);
        Vec3f::Max(bMax, bMax, v[2]);
    }
    for (int i = 0; i < g->Nx(); i++) {
        for (int j = 0; j < g->Ny(); j++) {
            for (int k = 0; k < g->Nz(); k++) {
                Vec3f vMin = g->getVoxelMin(i, j, k),
                      vMax = g->getVoxelMax(i, j, k);
                if (vMin.x() <= bMax.x() && vMax.x() >= bMin.x() &&
                    vMin.y() <= bMax.y() && vMax.y() >= bMin.y() &&
                    vMin.z() <= bMax.z() && vMax.z() >= bMin.z()) {
                    g->setVoxel(i, j, k, t ? (Object3D *)t : this);
                }
            }
        }
    }
}
