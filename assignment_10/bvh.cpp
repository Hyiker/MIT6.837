#include "bvh.hpp"

#include <algorithm>
#include <glincludes.hpp>
#include <random>
void BVHNode::build(Object3D** objs, int n) {
    int rand_axis = rand() % 3;
    auto cmp = [rand_axis](Object3D* a, Object3D* b) {
        return a->getBoundingBox()->getMin()[rand_axis] <
               b->getBoundingBox()->getMin()[rand_axis];
    };
    sort(objs, objs + n, cmp);
    if (n == 1) {
        m_left = m_right = objs[0];
    } else if (n == 2) {
        m_left = objs[0];
        m_right = objs[1];
    } else {
        auto left = new BVHNode();
        auto right = new BVHNode();
        left->build(objs, n / 2);
        right->build(objs + n / 2, n - n / 2);
        m_left = left;
        m_right = right;
    }
    m_bb = new BoundingBox(*m_left->getBoundingBox());
    m_bb->Extend(m_right->getBoundingBox());
}

bool BVHNode::intersect(const Ray& r, Hit& h, float tmin) {
    // intersect with bounding box

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

    float tBoxMin = std::min(tNear, tFar);
    if (tBoxMin > h.getT()) return false;
    bool inter0 = m_left->intersect(r, h, tmin),
         inter1 = m_right->intersect(r, h, tmin);
    return inter0 || inter1;
}
void BVHNode::paint() const { paint(0); }
void BVHNode::paint(int depth) const {
    constexpr int maxDepth = 6;
    if (depth > maxDepth) return;
    // paint the bounding box
    Vec3f min = m_bb->getMin(), max = m_bb->getMax();
    glColor3f(0.8, 0.0, 0.0);
    glLineWidth(1.0);
    glDisable(GL_LIGHTING);
    glBegin(GL_LINES);

    glVertex3f(min.x(), min.y(), min.z());
    glVertex3f(max.x(), min.y(), min.z());
    glVertex3f(min.x(), min.y(), min.z());
    glVertex3f(min.x(), max.y(), min.z());
    glVertex3f(max.x(), max.y(), min.z());
    glVertex3f(max.x(), min.y(), min.z());
    glVertex3f(max.x(), max.y(), min.z());
    glVertex3f(min.x(), max.y(), min.z());

    glVertex3f(min.x(), min.y(), min.z());
    glVertex3f(min.x(), min.y(), max.z());
    glVertex3f(min.x(), max.y(), min.z());
    glVertex3f(min.x(), max.y(), max.z());
    glVertex3f(max.x(), min.y(), min.z());
    glVertex3f(max.x(), min.y(), max.z());
    glVertex3f(max.x(), max.y(), min.z());
    glVertex3f(max.x(), max.y(), max.z());

    glVertex3f(min.x(), min.y(), max.z());
    glVertex3f(max.x(), min.y(), max.z());
    glVertex3f(min.x(), min.y(), max.z());
    glVertex3f(min.x(), max.y(), max.z());
    glVertex3f(max.x(), max.y(), max.z());
    glVertex3f(max.x(), min.y(), max.z());
    glVertex3f(max.x(), max.y(), max.z());
    glVertex3f(min.x(), max.y(), max.z());
    glEnd();
    if (auto left = dynamic_cast<BVHNode*>(m_left); left) {
        left->paint(depth + 1);
    }
    if (auto right = dynamic_cast<BVHNode*>(m_right); right) {
        right->paint(depth + 1);
    }
    glEnable(GL_LIGHTING);
}

void BVH::build(std::vector<Object3D*>& objs) {
    m_root = new BVHNode();
    m_root->build(objs.data(), objs.size());
}
