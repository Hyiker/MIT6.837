#ifndef ASSIGNMENT8_SPLINE_CURVE_HPP
#define ASSIGNMENT8_SPLINE_CURVE_HPP
#include <vector>

#include "spline.h"
class Curve : public Spline {
   public:
    Curve(int num_vertices) : m_nv(num_vertices), m_v(num_vertices) {}
    void set(int i, Vec3f v) { m_v[i] = v; }
    void Paint(ArgParser *args) override;
    int getNumVertices() const override { return m_nv; }
    // G
    Vec3f getVertex(int i) const override { return m_v[i]; }
    // Spline Basis
    virtual float BT(int i, float t) const = 0;
    virtual Vec3f Q(int offset, float t) const = 0;
    virtual std::vector<int> getOffsets() const = 0;
    void OutputBezier(FILE *file) override;
    void OutputBSpline(FILE *file) override;
    virtual std::vector<Vec3f> ConvertToBezier() const { return m_v; }
    virtual std::vector<Vec3f> ConvertToBSpline() const { return m_v; }
    // Editing Operations
    void moveControlPoint(int selectedPoint, float x, float y) override;
    void addControlPoint(int selectedPoint, float x, float y) override;
    void deleteControlPoint(int selectedPoint) override;

   protected:
    int m_nv;
    std::vector<Vec3f> m_v;
};
class BezierCurve : public Curve {
   public:
    BezierCurve(int num_vertices) : Curve(num_vertices) {}
    float BT(int i, float t) const override;
    Vec3f Q(int offset, float t) const override;
    std::vector<int> getOffsets() const override {
        std::vector<int> offsets;
        for (int i = 0; i < getNumVertices(); i += 3) {
            offsets.push_back(i);
        }
        offsets.pop_back();
        return offsets;
    }
    std::vector<Vec3f> ConvertToBSpline() const override;
};
class BSplineCurve : public Curve {
   public:
    BSplineCurve(int num_vertices) : Curve(num_vertices) {}
    float BT(int i, float t) const override;
    Vec3f Q(int offset, float t) const override;
    std::vector<int> getOffsets() const override {
        std::vector<int> offsets;
        for (int i = 0; i < getNumVertices() - 3; i++) {
            offsets.push_back(i);
        }
        return offsets;
    }
    std::vector<Vec3f> ConvertToBezier() const override;
};

#endif /* ASSIGNMENT8_SPLINE_CURVE_HPP */
