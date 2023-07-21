#include "curve.h"

#include <stdio.h>

#include <cmath>
#include <glincludes.hpp>
#include <iostream>
#include <vector>

#include "matrix.h"
void Curve::Paint(ArgParser *args) {
    // link control points
    glLineWidth(1);
    glColor3f(0, 0, 1);
    glBegin(GL_LINES);
    for (int i = 1; i < m_nv; i++) {
        Vec3f v1 = m_v[i - 1], v2 = m_v[i];
        glVertex3f(v1.x(), v1.y(), v1.z());
        glVertex3f(v2.x(), v2.y(), v2.z());
    }
    glEnd();
    // assume line segments count = curve_tessellation
    float dt = 1.0 / args->curve_tessellation;
    Vec3f plast;
    glLineWidth(3);
    glColor3f(0, 1, 0);
    glBegin(GL_LINES);
    for (int offset : getOffsets()) {
        float t = 0;
        for (int i = 0; i < args->curve_tessellation + 1; i++, t += dt) {
            Vec3f p = Q(offset, t);
            if (i > 0) {
                glVertex3f(plast.x(), plast.y(), plast.z());
                glVertex3f(p.x(), p.y(), p.z());
            }
            plast = p;
        }
    }
    glEnd();

    // render control points
    glPointSize(5);
    glColor3f(1, 0, 0);
    glBegin(GL_POINTS);
    for (int i = 0; i < m_nv; i++) {
        Vec3f v = getVertex(i);
        glVertex3f(v.x(), v.y(), v.z());
    }
    glEnd();
}
void Curve::OutputBezier(FILE *file) {
    fprintf(file, "bezier\nnum_vertices %d\n", getNumVertices());
    for (Vec3f v : ConvertToBezier()) {
        fprintf(file, "%f %f %f\n", v.x(), v.y(), v.z());
    }
}
void Curve::OutputBSpline(FILE *file) {
    fprintf(file, "bspline\nnum_vertices %d\n", getNumVertices());
    for (Vec3f v : ConvertToBSpline()) {
        fprintf(file, "%f %f %f\n", v.x(), v.y(), v.z());
    }
}

void Curve::moveControlPoint(int selectedPoint, float x, float y) {
    Vec3f v = getVertex(selectedPoint);
    v.Set(x, y, 0);
    set(selectedPoint, v);
}

void Curve::addControlPoint(int selectedPoint, float x, float y) {
    Vec3f v = getVertex(selectedPoint);
    v.Set(x, y, 0);
    m_v.insert(m_v.begin() + selectedPoint, v);
    m_nv++;
}

void Curve::deleteControlPoint(int selectedPoint) {
    m_v.erase(m_v.begin() + selectedPoint);
    m_nv--;
}

float BezierCurve::BT(int i, float t) const {
    assert(i >= 0 && i < 4);
    if (i == 0) {
        return pow(1 - t, 3);
    } else if (i == 1) {
        return 3 * t * pow(1 - t, 2);
    } else if (i == 2) {
        return 3 * pow(t, 2) * (1 - t);
    } else {
        return pow(t, 3);
    }
}
Vec3f BezierCurve::Q(int offset, float t) const {
    Vec3f q;
    for (int i = 0; i < 4; i++) {
        q += getVertex(i + offset) * BT(i, t);
    }
    return q;
}
static Matrix bezierBasis() {
    Matrix B;
    B.setRow(0, Vec4f(-1, 3, -3, 1));
    B.setRow(1, Vec4f(3, -6, 3, 0));
    B.setRow(2, Vec4f(-3, 3, 0, 0));
    B.setRow(3, Vec4f(1, 0, 0, 0));
    return B;
}
static Matrix bsplineBasis() {
    Matrix B;
    B.setRow(0, Vec4f(-1, 3, -3, 1));
    B.setRow(1, Vec4f(3, -6, 0, 4));
    B.setRow(2, Vec4f(-3, 3, 3, 1));
    B.setRow(3, Vec4f(1, 0, 0, 0));
    B *= 1.0 / 6.0;
    return B;
}

std::vector<Vec3f> BezierCurve::ConvertToBSpline() const {
    // only support 4 control points by now
    assert(getNumVertices() == 4);
    std::vector<Vec3f> bspline(4);
    Matrix G_beizer;
    for (int i = 0; i < 4; i++) {
        G_beizer.setColumn(i, Vec4f(getVertex(i), 0));
    }
    Matrix B_bspline_inv = bsplineBasis();
    B_bspline_inv.Inverse();
    Matrix B_bezier = bezierBasis();
    Matrix G_bspline = G_beizer * B_bezier * B_bspline_inv;
    for (int i = 0; i < 4; i++) {
        Vec4f v = G_bspline.getColumn(i);
        bspline[i] = Vec3f(v.x(), v.y(), v.z());
    }
    return bspline;
}

float BSplineCurve::BT(int i, float t) const {
    assert(i >= 0 && i < 4);
    if (i == 0) {
        return pow(1 - t, 3) / 6;
    } else if (i == 1) {
        return (3 * pow(t, 3) - 6 * pow(t, 2) + 4) / 6;
    } else if (i == 2) {
        return (-3 * pow(t, 3) + 3 * pow(t, 2) + 3 * t + 1) / 6;
    } else {
        return pow(t, 3) / 6;
    }
}
Vec3f BSplineCurve::Q(int offset, float t) const {
    Vec3f q;
    // evaluate 4 control points at a time
    for (int j = 0; j < 4; j++) {
        int s = offset + j;
        q += getVertex(s) * BT(j, t);
    }

    return q;
}
std::vector<Vec3f> BSplineCurve::ConvertToBezier() const {
    // only support 4 control points by now
    assert(getNumVertices() == 4);
    std::vector<Vec3f> bezier(4);
    Matrix G_bspline;
    for (int i = 0; i < 4; i++) {
        G_bspline.setColumn(i, Vec4f(getVertex(i), 0));
    }
    Matrix B_bezier_inv = bezierBasis();
    B_bezier_inv.Inverse();
    Matrix B_bspline = bsplineBasis();
    Matrix G_bezier = G_bspline * B_bspline * B_bezier_inv;
    for (int i = 0; i < 4; i++) {
        Vec4f v = G_bezier.getColumn(i);
        bezier[i] = Vec3f(v.x(), v.y(), v.z());
    }
    return bezier;
}