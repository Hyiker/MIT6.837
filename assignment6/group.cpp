#include "group.h"
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
void Group::insertIntoGrid(Grid *g, Matrix *m) {
    for (int i = 0; i < m_num_objects; i++) {
        m_objects[i]->insertIntoGrid(g, m);
    }
}
