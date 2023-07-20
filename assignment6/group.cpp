#include "group.h"

#include <vector>

#include "transform.h"
bool Group::intersect(const Ray &r, Hit &h, float tmin) {
    bool result = false;
    for (int i = 0; i < m_num_objects; i++) {
        result = m_objects[i]->intersect(r, h, tmin) || result;
    }
    return result;
}
void Group::paint() const {
    for (int i = 0; i < m_num_objects; i++) {
        m_objects[i]->paint();
    }
}
void Group::insertIntoGrid(Grid *g, Matrix *m, Transform *t) {
    assert(t == nullptr);
    for (int i = 0; i < m_num_objects; i++) {
        m_objects[i]->insertIntoGrid(g, m);
    }
}
static void dfsFlatten(Object3D *node, Matrix m,
                       std::vector<Object3D *> &result,
                       bool metTransform = false) {
    if (auto t = dynamic_cast<Transform *>(node); t) {
        m = m * t->getMatrix();
        dfsFlatten(t->getObject(), m, result, true);
    } else if (auto g = dynamic_cast<Group *>(node); g) {
        for (auto &object : g->getObjects()) {
            dfsFlatten(object, m, result, metTransform);
        }
    } else {
        result.push_back(metTransform ? new Transform(m, node) : node);
    }
}
void Group::flatten() {
    std::vector<Object3D *> objects;
    Matrix identity;
    identity.SetToIdentity();
    // dfs the group tree
    for (int i = 0; i < m_num_objects; i++) {
        dfsFlatten(m_objects[i], identity, objects);
    }
    // remove the old objects
    delete[] m_objects;
    // set the new objects
    m_num_objects = objects.size();
    m_objects = new Object3D *[m_num_objects];
    for (int i = 0; i < m_num_objects; i++) {
        m_objects[i] = objects[i];
    }
}