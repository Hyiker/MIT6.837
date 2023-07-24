#ifndef ASSIGNMENT_10_INCLUDE_BVH_HPP
#define ASSIGNMENT_10_INCLUDE_BVH_HPP
#include <vector>

#include "object3d.h"
#include "ray.h"
class BVHNode : public Object3D {
   public:
    BVHNode() {}
    void build(Object3D** objs, int n);
    bool intersect(const Ray& r, Hit& h, float tmin) override;
    void paint() const override;
    void paint(int depth) const;
    ~BVHNode() override {
        delete m_left;
        delete m_right;
    }

   private:
    Object3D* m_left{};
    Object3D* m_right{};
};

class BVH {
   public:
    BVH() {}
    void build(std::vector<Object3D*>& objs);
    ~BVH() { delete m_root; }
    bool intersect(const Ray& r, Hit& h, float tmin) const {
        bool hit = false;
        for (auto obj : infiniteObjects) {
            hit |= obj->intersect(r, h, tmin);
        }
        return m_root->intersect(r, h, tmin) || hit;
    }
    void paint() const {
        assert(m_root != nullptr);
        m_root->paint();
    }

    std::vector<Object3D*> infiniteObjects;

   private:
    BVHNode* m_root;
};

#endif /* ASSIGNMENT_10_INCLUDE_BVH_HPP */
