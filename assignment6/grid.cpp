#include "grid.h"

#include <glincludes.hpp>

#include "rayTree.h"
#include "raytracing_stats.h"
void MarchingInfo::nextCell() {
    float t_next_min = std::min(t_next.x(), std::min(t_next.y(), t_next.z()));
    if (t_next_min == t_next.x()) {
        i += sign_x;
        t_next.x() += d_t.x();
        normal = Vec3f(-sign_x, 0, 0);
    } else if (t_next_min == t_next.y()) {
        j += sign_y;
        t_next.y() += d_t.y();
        normal = Vec3f(0, -sign_y, 0);
    } else {
        k += sign_z;
        t_next.z() += d_t.z();
        normal = Vec3f(0, 0, -sign_z);
    }
    tmin = t_next_min;
}

bool Grid::intersect(const Ray &r, Hit &h, float tmin) {
    MarchingInfo mi;
    initializeRayMarch(mi, r, tmin);
    Vec3f delta((m_bb->getMax().x() - m_bb->getMin().x()) / m_nx / 2.0f,
                (m_bb->getMax().y() - m_bb->getMin().y()) / m_ny / 2.0f,
                (m_bb->getMax().z() - m_bb->getMin().z()) / m_nz / 2.0f);
    Vec3f faces[6][4]{
        // +x
        {Vec3f(1, 1, 1), Vec3f(1, 1, -1), Vec3f(1, -1, -1), Vec3f(1, -1, 1)},
        // -x
        {Vec3f(-1, 1, 1), Vec3f(-1, 1, -1), Vec3f(-1, -1, -1),
         Vec3f(-1, -1, 1)},
        // +y
        {Vec3f(1, 1, 1), Vec3f(1, 1, -1), Vec3f(-1, 1, -1), Vec3f(-1, 1, 1)},
        // -y
        {Vec3f(1, -1, 1), Vec3f(1, -1, -1), Vec3f(-1, -1, -1),
         Vec3f(-1, -1, 1)},
        // +z
        {Vec3f(1, 1, 1), Vec3f(1, -1, 1), Vec3f(-1, -1, 1), Vec3f(-1, 1, 1)},
        // -z
        {Vec3f(1, 1, -1), Vec3f(1, -1, -1), Vec3f(-1, -1, -1),
         Vec3f(-1, 1, -1)},
    };
    Vec3f faceNormals[6]{
        Vec3f(1, 0, 0),  Vec3f(-1, 0, 0), Vec3f(0, 1, 0),
        Vec3f(0, -1, 0), Vec3f(0, 0, 1),  Vec3f(0, 0, -1),
    };
    bool first = false;
    if (!mi.hit) return false;
    Vec3f colors[3] = {Vec3f(1, 0, 0), Vec3f(0, 1, 0), Vec3f(0, 0, 1)};
    int icolor = 0;
    while (isInside(mi.i, mi.j, mi.k)) {
        int i = mi.i, j = mi.j, k = mi.k;
        Material *m = nullptr;
        if (auto obj = getVoxel(i, j, k); obj) {
            m = obj->getMaterial();
            if (!first) {
                first = true;
                h.set(mi.tmin, m, mi.normal, r);
            }
        }
        // update ray tree
        Vec3f center = getVoxelCenter(i, j, k);
        Material *boxm =
            new PhongMaterial(colors[icolor], Vec3f(0, 0, 0), 0.0,
                              Vec3f(0.0, 0.0, 0.0), Vec3f(0.0, 0.0, 0.0), 1.0);
        for (int i = 0; i < 6; i++) {
            Vec3f face[4];
            for (int j = 0; j < 4; j++) {
                face[j] = faces[i][j] * delta + center;
            }
            RayTree::AddHitCellFace(face[0], face[1], face[2], face[3],
                                    faceNormals[i], boxm);
        }
        icolor = (icolor + 1) % 3;
        int ienter = 0;
        if (mi.normal.x() == 1) {
            ienter = 0;
        } else if (mi.normal.x() == -1) {
            ienter = 1;
        } else if (mi.normal.y() == 1) {
            ienter = 2;
        } else if (mi.normal.y() == -1) {
            ienter = 3;
        } else if (mi.normal.z() == 1) {
            ienter = 4;
        } else if (mi.normal.z() == -1) {
            ienter = 5;
        }
        Vec3f face[4];
        for (int j = 0; j < 4; j++) {
            face[j] = faces[ienter][j] * delta + center;
        }
        RayTree::AddEnteredFace(face[0], face[1], face[2], face[3],
                                faceNormals[ienter], boxm);
        // count cell traversal
        RayTracingStats::IncrementNumGridCellsTraversed();
        mi.nextCell();
    }
    return first;
}
void Grid::initializeRayMarch(MarchingInfo &mi, const Ray &r,
                              float tmin) const {
    Vec3f offset = r.getOrigin() - m_bb->getMin();
    Vec3f voxelSize = getVoxelSize();
    Vec3f d_t = voxelSize;
    Vec3f rd = r.getDirection();
    d_t.Divide(abs(rd.x()), abs(rd.y()), abs(rd.z()));
    mi.sign_x = rd.x() > 0 ? 1 : -1;
    mi.sign_y = rd.y() > 0 ? 1 : -1;
    mi.sign_z = rd.z() > 0 ? 1 : -1;
    if (rd.x() == 0) mi.sign_x = 0;
    if (rd.y() == 0) mi.sign_y = 0;
    if (rd.z() == 0) mi.sign_z = 0;
    mi.d_t = d_t;
    Vec3f origin = r.getOrigin();
    if (m_bb->isInside(origin)) {
        // ray origin is inside the grid
        getVoxelIndexAtPoint(origin, mi.i, mi.j, mi.k);

    } else {
        // ray origin outside
        if (!intersectBox(r, tmin, mi.tmin, mi.normal)) {
            mi.hit = false;
            return;
        }
        Vec3f hitPoint = r.pointAtParameter(mi.tmin);
        getVoxelIndexAtPoint(hitPoint, mi.i, mi.j, mi.k);
    }
    mi.hit = true;
    mi.t_next.x() = (m_bb->getMin().x() +
                     voxelSize.x() * (mi.i + (mi.sign_x == -1 ? 0 : 1)) -
                     r.getOrigin().x()) /
                    rd.x();
    mi.t_next.y() = (m_bb->getMin().y() +
                     voxelSize.y() * (mi.j + (mi.sign_y == -1 ? 0 : 1)) -
                     r.getOrigin().y()) /
                    rd.y();
    mi.t_next.z() = (m_bb->getMin().z() +
                     voxelSize.z() * (mi.k + (mi.sign_z == -1 ? 0 : 1)) -
                     r.getOrigin().z()) /
                    rd.z();
}
void Grid::paint() const {
    float delta_x_2 = (m_bb->getMax().x() - m_bb->getMin().x()) / m_nx / 2.0f,
          delta_y_2 = (m_bb->getMax().y() - m_bb->getMin().y()) / m_ny / 2.0f,
          delta_z_2 = (m_bb->getMax().z() - m_bb->getMin().z()) / m_nz / 2.0f;
    // paint each voxel as 6 quad faces
    for (int i = 0; i < m_nx; i++) {
        for (int j = 0; j < m_ny; j++) {
            for (int k = 0; k < m_nz; k++) {
                if (auto o = getVoxel(i, j, k); o) {
                    o->getMaterial()->glSetMaterial();
                    Vec3f center = getVoxelCenter(i, j, k);
                    float x = center.x();
                    float y = center.y();
                    float z = center.z();
                    glBegin(GL_QUADS);
                    // +x face
                    glNormal3f(1.0f, 0.0f, 0.0f);
                    glVertex3f(x + delta_x_2, y + delta_y_2, z + delta_z_2);
                    glVertex3f(x + delta_x_2, y - delta_y_2, z + delta_z_2);
                    glVertex3f(x + delta_x_2, y - delta_y_2, z - delta_z_2);
                    glVertex3f(x + delta_x_2, y + delta_y_2, z - delta_z_2);
                    // -x face
                    glNormal3f(-1.0f, 0.0f, 0.0f);
                    glVertex3f(x - delta_x_2, y + delta_y_2, z + delta_z_2);
                    glVertex3f(x - delta_x_2, y - delta_y_2, z + delta_z_2);
                    glVertex3f(x - delta_x_2, y - delta_y_2, z - delta_z_2);
                    glVertex3f(x - delta_x_2, y + delta_y_2, z - delta_z_2);
                    // +y face
                    glNormal3f(0.0f, 1.0f, 0.0f);
                    glVertex3f(x + delta_x_2, y + delta_y_2, z + delta_z_2);
                    glVertex3f(x + delta_x_2, y + delta_y_2, z - delta_z_2);
                    glVertex3f(x - delta_x_2, y + delta_y_2, z - delta_z_2);
                    glVertex3f(x - delta_x_2, y + delta_y_2, z + delta_z_2);
                    // -y face
                    glNormal3f(0.0f, -1.0f, 0.0f);
                    glVertex3f(x + delta_x_2, y - delta_y_2, z + delta_z_2);
                    glVertex3f(x + delta_x_2, y - delta_y_2, z - delta_z_2);
                    glVertex3f(x - delta_x_2, y - delta_y_2, z - delta_z_2);
                    glVertex3f(x - delta_x_2, y - delta_y_2, z + delta_z_2);
                    // +z face
                    glNormal3f(0.0f, 0.0f, 1.0f);
                    glVertex3f(x + delta_x_2, y + delta_y_2, z + delta_z_2);
                    glVertex3f(x - delta_x_2, y + delta_y_2, z + delta_z_2);
                    glVertex3f(x - delta_x_2, y - delta_y_2, z + delta_z_2);
                    glVertex3f(x + delta_x_2, y - delta_y_2, z + delta_z_2);
                    // -z face
                    glNormal3f(0.0f, 0.0f, -1.0f);
                    glVertex3f(x + delta_x_2, y + delta_y_2, z - delta_z_2);
                    glVertex3f(x - delta_x_2, y + delta_y_2, z - delta_z_2);
                    glVertex3f(x - delta_x_2, y - delta_y_2, z - delta_z_2);
                    glVertex3f(x + delta_x_2, y - delta_y_2, z - delta_z_2);
                    glEnd();
                }
            }
        }
    }
}
bool Grid::intersectBox(const Ray &r, float tmin, float &t,
                        Vec3f &normal) const {
    Vec3f tMin = (m_bb->getMin() - r.getOrigin()),
          tMax = (m_bb->getMax() - r.getOrigin());
    Vec3f dir = r.getDirection();
    tMin.Divide(dir.x(), dir.y(), dir.z());
    tMax.Divide(dir.x(), dir.y(), dir.z());
    Vec3f t1, t2;
    Vec3f::Min(t1, tMin, tMax);
    Vec3f::Max(t2, tMin, tMax);
    float tNear = std::max(std::max(t1.x(), t1.y()), t1.z()),
          tFar = std::min(std::min(t2.x(), t2.y()), t2.z());
    if (tNear > tFar) {
        return false;
    }
    if (tNear > tmin) {
        t = tNear;
    } else {
        t = tFar;
    }
    if (tNear == t1.x()) {
        normal = Vec3f(1.0f, 0.0f, 0.0f);
    } else if (tNear == t1.y()) {
        normal = Vec3f(0.0f, 1.0f, 0.0f);
    } else if (tNear == t1.z()) {
        normal = Vec3f(0.0f, 0.0f, 1.0f);
    } else if (tNear == t2.x()) {
        normal = Vec3f(-1.0f, 0.0f, 0.0f);
    } else if (tNear == t2.y()) {
        normal = Vec3f(0.0f, -1.0f, 0.0f);
    } else if (tNear == t2.z()) {
        normal = Vec3f(0.0f, 0.0f, -1.0f);
    }
    return true;
}
