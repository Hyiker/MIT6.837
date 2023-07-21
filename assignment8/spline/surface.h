#ifndef ASSIGNMENT8_SPLINE_SURFACE_HPP
#define ASSIGNMENT8_SPLINE_SURFACE_HPP
#include "spline.h"
class Surface : public Spline {
   public:
};
class SurfaceOfRevolution : public Surface {
   public:
    SurfaceOfRevolution(Curve* c) : m_curve(c) {}
    TriangleMesh* OutputTriangles(ArgParser* args) override;
    void Paint(ArgParser* args) override;

    int getNumVertices() const override;
    Vec3f getVertex(int i) const override;

    void moveControlPoint(int selectedPoint, float x, float y) override;
    void addControlPoint(int selectedPoint, float x, float y) override;
    void deleteControlPoint(int selectedPoint) override;
    void OutputBezier(FILE* file) override;
    void OutputBSpline(FILE* file) override;

   private:
    Curve* m_curve;
};
class BezierPatch : public Surface {
   public:
    BezierPatch() : m_v(16) {}
    int getNumVertices() const override { return m_v.size(); }
    Vec3f getVertex(int i) const override { return m_v[i]; }
    TriangleMesh* OutputTriangles(ArgParser* args) override;
    void set(int i, const Vec3f& v);

   private:
    std::vector<Vec3f> m_v;
};
#endif /* ASSIGNMENT8_SPLINE_SURFACE_HPP */
