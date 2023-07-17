#ifndef ASSIGNMENT1_GROUP_HPP
#define ASSIGNMENT1_GROUP_HPP
#include "object3d.h"
class Group : public Object3D {
   public:
    Group(int num_objects) : m_num_objects(num_objects) {
        m_objects = new Object3D *[num_objects];
        for (int i = 0; i < num_objects; i++) {
            m_objects[i] = nullptr;
        }
    }
    ~Group() override = default;
    bool intersect(const Ray &r, Hit &h, float tmin) override;
    void addObject(int index, Object3D *obj) { m_objects[index] = obj; }

   private:
    int m_num_objects;
    Object3D **m_objects{};
};

#endif /* ASSIGNMENT1_GROUP_HPP */
