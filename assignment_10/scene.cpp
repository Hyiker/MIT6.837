#include "scene.hpp"

#include <unordered_set>

#include "group.h"
#include "plane.h"
#include "raytracing_stats.h"
#include "transform.h"
Scene::Scene(SceneParser* scene_parser, Grid* grid, BVH* bvh)
    : m_scene_parser(scene_parser), m_grid(grid), m_bvh(bvh) {
    m_scene_parser->getGroup()->flatten();
    auto group = m_scene_parser->getGroup();
    if (m_grid) {
        group->insertIntoGrid(grid, nullptr);
    }
    if (m_bvh) {
        std::vector<Object3D*> objs;
        for (Object3D* obj : group->getObjects()) {
            bool isInfinite = false;
            if (auto transform = dynamic_cast<Transform*>(obj); transform) {
                isInfinite = dynamic_cast<Plane*>(transform->getObject());
            } else {
                isInfinite = dynamic_cast<Plane*>(obj);
            }
            if (isInfinite)
                m_bvh->infiniteObjects.push_back(obj);
            else
                objs.push_back(obj);
        }

        m_bvh->build(objs);
        std::cout << "BVH built" << std::endl;
    }
}

bool Scene::intersect(const Ray& r, Hit& h, float tmin) const {
    if (m_grid)
        return intersectFastGrid(r, h, tmin);
    else if (m_bvh)
        return intersectFastBVH(r, h, tmin);
    return m_scene_parser->getGroup()->intersect(r, h, tmin);
}
bool Scene::intersectFastBVH(const Ray& r, Hit& h, float tmin) const {
    return m_bvh->intersect(r, h, tmin);
}
bool Scene::intersectFastGrid(const Ray& r, Hit& h, float tmin) const {
    std::unordered_set<Object3D*> passBys;
    // check infinite geometries first
    bool hit_fb = m_grid->intersectInfiniteObjects(r, h, tmin);
    MarchingInfo mi;
    m_grid->initializeRayMarch(mi, r, tmin);
    if (!mi.hit) return hit_fb;
    int ilast{-1}, jlast{-1}, klast{-1};
    while (m_grid->isInside(mi.i, mi.j, mi.k)) {
        if (mi.i == ilast && mi.j == jlast && mi.k == klast) {
            std::cerr << "infinite loop" << std::endl;
            std::cout << mi.i << " " << mi.j << " " << mi.k << std::endl;
            std::cout << mi.sign_x << " " << mi.sign_y << " " << mi.sign_z
                      << std::endl;
            std::cout << mi.t_next.x() << " " << mi.t_next.y() << " "
                      << mi.t_next.z() << std::endl;
            std::cout << mi.d_t.x() << " " << mi.d_t.y() << " " << mi.d_t.z()
                      << std::endl;
            std::cout << mi.tmin << std::endl;
            exit(1);
        }
        ilast = mi.i;
        jlast = mi.j;
        klast = mi.k;
        auto vec = m_grid->getVoxel(mi.i, mi.j, mi.k);
        bool hit = false;
        Vec3f vMin = m_grid->getVoxelMin(mi.i, mi.j, mi.k),
              vMax = m_grid->getVoxelMax(mi.i, mi.j, mi.k);
        for (auto obj : vec) {
            if (passBys.count(obj)) continue;
            RayTracingStats::IncrementNumIntersections();
            bool hitThis = obj->intersect(r, h, tmin);
            Vec3f hit_p = r.pointAtParameter(h.getT());
            if (!hitThis) {
                passBys.insert(obj);
                continue;
            }
            // if hit point is not within the voxel cell, continue
            bool isInsideVoxel =
                hit_p.x() >= vMin.x() && hit_p.y() >= vMin.y() &&
                hit_p.z() >= vMin.z() && hit_p.x() <= vMax.x() &&
                hit_p.y() <= vMax.y() && hit_p.z() <= vMax.z();
            if (isInsideVoxel) passBys.insert(obj);
            hit_fb = true;
            hit = isInsideVoxel || hit;
        }
        if (hit) return true;
        RayTracingStats::IncrementNumGridCellsTraversed();
        mi.nextCell();
    }
    return hit_fb;
}