#ifndef ASSIGNMENT8_SPLINE_SPLINE_HPP
#define ASSIGNMENT8_SPLINE_SPLINE_HPP
#include "argparser.hpp"
#include "triangle_mesh.hpp"
#include "vectors.h"
class Spline {
   public:
    // FOR VISUALIZATION
    virtual void Paint(ArgParser *args) {}

    // TODO FOR CONVERTING BETWEEN SPLINE TYPES
    virtual void OutputBezier(FILE *file) {}
    virtual void OutputBSpline(FILE *file) {}

    // TODO FOR CONTROL POINT PICKING
    virtual int getNumVertices() const { return 0; }
    virtual Vec3f getVertex(int i) const { return Vec3f(); }

    // TODO FOR EDITING OPERATIONS
    virtual void moveControlPoint(int selectedPoint, float x, float y) {}
    virtual void addControlPoint(int selectedPoint, float x, float y) {}
    virtual void deleteControlPoint(int selectedPoint) {}

    // FOR GENERATING TRIANGLES
    virtual TriangleMesh *OutputTriangles(ArgParser *args) {
        // TODO
        return nullptr;
    }
};
class Curve;
class BezierCurve;
class BSplineCurve;
class Surface;
class SurfaceOfRevolution;
class BezierPatch;
Vec3f BezierCurveQ(const Vec3f &v0, const Vec3f &v1, const Vec3f &v2,
                   const Vec3f &v3, float t);

#endif /* ASSIGNMENT8_SPLINE_SPLINE_HPP */
