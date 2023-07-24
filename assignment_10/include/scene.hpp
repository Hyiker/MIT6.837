#ifndef ASSIGNMENT_10_INCLUDE_SCENE_HPP
#define ASSIGNMENT_10_INCLUDE_SCENE_HPP
#include "bvh.hpp"
#include "grid.h"
#include "scene_parser.h"
class Scene {
   public:
    Scene(SceneParser* scene_parser, Grid* grid = nullptr, BVH* bvh = nullptr);
    ~Scene() {
        delete m_scene_parser;
        delete m_grid;
    }

    bool intersect(const Ray& r, Hit& h, float tmin) const;
    bool intersectFastGrid(const Ray& r, Hit& h, float tmin) const;
    bool intersectFastBVH(const Ray& r, Hit& h, float tmin) const;
    auto getCamera() const { return m_scene_parser->getCamera(); }
    auto getAmbientLight() const { return m_scene_parser->getAmbientLight(); }
    auto getLight(int i) const { return m_scene_parser->getLight(i); }
    auto getNumLights() const { return m_scene_parser->getNumLights(); }
    auto getBackgroundColor() const {
        return m_scene_parser->getBackgroundColor();
    }
    auto getEmitters() const { return m_scene_parser->getEmitters(); }

   private:
    SceneParser* m_scene_parser;
    Grid* m_grid;
    BVH* m_bvh;
};

#endif /* ASSIGNMENT_10_INCLUDE_SCENE_HPP */
