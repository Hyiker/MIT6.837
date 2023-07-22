#ifndef ASSIGNMENT5_GROUP_HPP
#define ASSIGNMENT5_GROUP_HPP
#include <vector>

#include "object3d.h"
class Group : public Object3D {
   public:
    Group(int num_objects) : m_num_objects(num_objects) {
        m_objects = new Object3D *[num_objects];
        for (int i = 0; i < num_objects; i++) {
            m_objects[i] = nullptr;
        }
        m_bb = new BoundingBox(Vec3f(INFINITY, INFINITY, INFINITY),
                               Vec3f(-INFINITY, -INFINITY, -INFINITY));
    }
    ~Group() override = default;
    bool intersect(const Ray &r, Hit &h, float tmin) override;
    void addObject(int index, Object3D *obj) {
        m_objects[index] = obj;
        if (obj->getBoundingBox() != nullptr)
            m_bb->Extend(obj->getBoundingBox());
    }
    void paint() const override;
    void insertIntoGrid(Grid *g, Matrix *m, Transform *t = nullptr) override;
    std::vector<Object3D *> getObjects() const {
        std::vector<Object3D *> objects;
        for (int i = 0; i < m_num_objects; i++) {
            objects.push_back(m_objects[i]);
        }
        return objects;
    }
    void flatten();

   private:
    int m_num_objects;
    Object3D **m_objects{};
};

#endif /* ASSIGNMENT5_GROUP_HPP */
