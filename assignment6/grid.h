#ifndef ASSIGNMENT5_GRID_HPP
#define ASSIGNMENT5_GRID_HPP
#include <vector>

#include "object3d.h"
struct MarchingInfo {
   public:
    MarchingInfo() = default;
    void nextCell();

    // surface normal of the entered cell to cross
    Vec3f normal;

    float tmin;
    int i, j, k;
    // next values of intersection along each axis
    Vec3f t_next;
    // marching increments along each axis
    Vec3f d_t;
    // sign of marching increments along each axis
    int sign_x, sign_y, sign_z;
    bool hit{false};
};
class Grid : public Object3D {
   public:
    Grid(BoundingBox *bb, int nx, int ny, int nz)
        : Object3D(),
          m_nx(nx),
          m_ny(ny),
          m_nz(nz),
          m_cells(nx * ny * nz, nullptr) {
        m_bb = bb;
    }
    ~Grid() override = default;
    void initializeRayMarch(MarchingInfo &mi, const Ray &r, float tmin) const;
    bool intersect(const Ray &r, Hit &h, float tmin) override;
    void paint() const override;
    Vec3f getVoxelCenter(int i, int j, int k) const {
        float x = m_bb->getMin().x() +
                  (i + 0.5) * (m_bb->getMax().x() - m_bb->getMin().x()) / m_nx;
        float y = m_bb->getMin().y() +
                  (j + 0.5) * (m_bb->getMax().y() - m_bb->getMin().y()) / m_ny;
        float z = m_bb->getMin().z() +
                  (k + 0.5) * (m_bb->getMax().z() - m_bb->getMin().z()) / m_nz;
        return Vec3f(x, y, z);
    }
    Vec3f getVoxelMin(int i, int j, int k) const {
        float x = m_bb->getMin().x() +
                  i * (m_bb->getMax().x() - m_bb->getMin().x()) / m_nx;
        float y = m_bb->getMin().y() +
                  j * (m_bb->getMax().y() - m_bb->getMin().y()) / m_ny;
        float z = m_bb->getMin().z() +
                  k * (m_bb->getMax().z() - m_bb->getMin().z()) / m_nz;
        return Vec3f(x, y, z);
    }
    Vec3f getVoxelMax(int i, int j, int k) const {
        return getVoxelMin(i + 1, j + 1, k + 1);
    }
    int Nx() const { return m_nx; }
    int Ny() const { return m_ny; }
    int Nz() const { return m_nz; }
    int Volume() const { return m_nx * m_ny * m_nz; }
    auto getVoxel(int i, int j, int k) const {
        return m_cells[i + m_nx * (j + m_ny * k)];
    }
    auto getVoxelFlat(int i) const { return m_cells[i]; }
    void setVoxel(int i, int j, int k, Object3D *value) {
        m_cells[i + m_nx * (j + m_ny * k)] = value;
    }
    Vec3f getVoxelSize() const {
        return Vec3f((m_bb->getMax().x() - m_bb->getMin().x()) / m_nx,
                     (m_bb->getMax().y() - m_bb->getMin().y()) / m_ny,
                     (m_bb->getMax().z() - m_bb->getMin().z()) / m_nz);
    }
    void getVoxelIndexAtPoint(const Vec3f &p, int &i, int &j, int &k) const {
        i = (int)floorf((p.x() - m_bb->getMin().x()) / getVoxelSize().x());
        j = (int)floorf((p.y() - m_bb->getMin().y()) / getVoxelSize().y());
        k = (int)floorf((p.z() - m_bb->getMin().z()) / getVoxelSize().z());
        i = std::min(std::max(i, 0), m_nx - 1);
        j = std::min(std::max(j, 0), m_ny - 1);
        k = std::min(std::max(k, 0), m_nz - 1);
    }
    bool intersectBox(const Ray &r, float tmin, float &t, Vec3f &normal) const;
    bool isInside(int i, int j, int k) const {
        return i >= 0 && i < m_nx && j >= 0 && j < m_ny && k >= 0 && k < m_nz;
    }
    void addInfiniteObject(Object3D *obj) { m_infinite_objs.push_back(obj); }

   private:
    int m_nx, m_ny, m_nz;
    std::vector<Object3D *> m_cells;
    std::vector<Object3D *> m_infinite_objs;
};

#endif /* ASSIGNMENT5_GRID_HPP */
