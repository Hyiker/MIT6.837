#include "surface.h"

#include <matrix.h>

#include <glincludes.hpp>

#include "curve.h"
void SurfaceOfRevolution::Paint(ArgParser* args) { m_curve->Paint(args); }

int SurfaceOfRevolution::getNumVertices() const {
    return m_curve->getNumVertices();
}
Vec3f SurfaceOfRevolution::getVertex(int i) const {
    return m_curve->getVertex(i);
}
TriangleMesh* SurfaceOfRevolution::OutputTriangles(ArgParser* args) {
    auto offsets = m_curve->getOffsets();
    int utess = args->revolution_tessellation,
        vtess = args->curve_tessellation * (offsets.size());

    TriangleNet* tn = new TriangleNet(utess, vtess);
    for (int i = 0; i <= utess; i++) {
        for (int j = 0; j <= vtess; j++) {
            float u = (float)i / utess;
            int ioffset = j / args->curve_tessellation;
            ioffset = std::min(ioffset, (int)offsets.size() - 1);
            int offset = offsets[ioffset];
            float t = (j - ioffset * args->curve_tessellation) /
                      float(args->curve_tessellation);
            float theta = u * 2 * M_PI;

            Matrix rot = Matrix::MakeYRotation(theta);
            Vec3f p = m_curve->Q(offset, t);
            rot.Transform(p);
            tn->SetVertex(i, j, p);
        }
    }
    return tn;
}

void SurfaceOfRevolution::moveControlPoint(int selectedPoint, float x,
                                           float y) {
    m_curve->moveControlPoint(selectedPoint, x, y);
}

void SurfaceOfRevolution::addControlPoint(int selectedPoint, float x, float y) {
    m_curve->addControlPoint(selectedPoint, x, y);
}

void SurfaceOfRevolution::deleteControlPoint(int selectedPoint) {
    m_curve->deleteControlPoint(selectedPoint);
}

void SurfaceOfRevolution::OutputBezier(FILE* file) {
    fprintf(file, "surface_of_revolution\nbezier\nnum_vertices %d\n",
            getNumVertices());
    for (Vec3f v : m_curve->ConvertToBezier()) {
        fprintf(file, "%f %f %f\n", v.x(), v.y(), v.z());
    }
}
void SurfaceOfRevolution::OutputBSpline(FILE* file) {
    fprintf(file, "surface_of_revolution\nbspline\nnum_vertices %d\n",
            getNumVertices());
    for (Vec3f v : m_curve->ConvertToBSpline()) {
        fprintf(file, "%f %f %f\n", v.x(), v.y(), v.z());
    }
}

TriangleMesh* BezierPatch::OutputTriangles(ArgParser* args) {
    int tess = args->patch_tessellation;

    TriangleNet* tn = new TriangleNet(tess, tess);
    for (int i = 0; i <= tess; i++) {
        for (int j = 0; j <= tess; j++) {
            float s = (float)i / tess, t = (float)j / tess;
            Vec3f p = BezierCurveQ(
                BezierCurveQ(m_v[0], m_v[1], m_v[2], m_v[3], s),
                BezierCurveQ(m_v[4], m_v[5], m_v[6], m_v[7], s),
                BezierCurveQ(m_v[8], m_v[9], m_v[10], m_v[11], s),
                BezierCurveQ(m_v[12], m_v[13], m_v[14], m_v[15], s), t);
            tn->SetVertex(i, j, p);
        }
    }
    return tn;
}
void BezierPatch::set(int i, const Vec3f& v) {
    assert(i >= 0 && i < 16);
    m_v[i] = v;
}