// ====================================================================
// Here are the prototypes for five pure virtual functions for
// initializing, placing, dollying, trucking, and rotating an
// OpenGL camera.  These should be added to the Camera class and
// each of its subclasses
// ====================================================================
// clang-format off
#include <Windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
// clang-format on
#include "camera.h"

#include <matrix.h>

// ====================================================================
// Below are the function bodies for
// each of these routines for the OrthographicCamera
// and PerspectiveCamera subclasses
// ====================================================================

// ====================================================================
// Create an orthographic camera with the appropriate dimensions that
// crops the screen in the narrowest dimension.
// ====================================================================

void OrthographicCamera::glInit(int w, int h) {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if (w > h)
        glOrtho(-m_size / 2.0, m_size / 2.0,
                -m_size * (float)h / (float)w / 2.0,
                m_size * (float)h / (float)w / 2.0, 0.5, 40.0);
    else
        glOrtho(-m_size * (float)w / (float)h / 2.0,
                m_size * (float)w / (float)h / 2.0, -m_size / 2.0, m_size / 2.0,
                0.5, 40.0);
}

// ====================================================================
// Place an orthographic camera within an OpenGL scene
// ====================================================================

void OrthographicCamera::glPlaceCamera(void) {
    gluLookAt(m_center.x(), m_center.y(), m_center.z(),
              m_center.x() + m_direction.x(), m_center.y() + m_direction.y(),
              m_center.z() + m_direction.z(), m_up.x(), m_up.y(), m_up.z());
}

// ====================================================================
// dollyCamera, truckCamera, and RotateCamera
//
// Asumptions:
//  - up is really up (i.e., it hasn't been changed
//    to point to "screen up")
//  - up and direction are normalized
// Special considerations:
//  - If your constructor precomputes any vectors for
//    use in 'generateRay', you will likely to recompute those
//    values at athe end of the these three routines
// ====================================================================

// ====================================================================
// dollyCamera: Move camera along the direction vector
// ====================================================================

void OrthographicCamera::dollyCamera(float dist) {
    m_center += m_direction * dist;

    // ===========================================
    // ASSIGNMENT 3: Fix any other affected values
    // ===========================================
}

// ====================================================================
// truckCamera: Translate camera perpendicular to the direction vector
// ====================================================================

void OrthographicCamera::truckCamera(float dx, float dy) {
    Vec3f horizontal;
    Vec3f::Cross3(horizontal, m_direction, m_up);
    horizontal.Normalize();

    Vec3f screenUp;
    Vec3f::Cross3(screenUp, horizontal, m_direction);

    m_center += horizontal * dx + screenUp * dy;

    // ===========================================
    // ASSIGNMENT 3: Fix any other affected values
    // ===========================================
    m_screenup = screenUp;
    m_horizontal = horizontal;
}

// ====================================================================
// rotateCamera: Rotate around the up and horizontal vectors
// ====================================================================

void OrthographicCamera::rotateCamera(float rx, float ry) {
    Vec3f horizontal;
    Vec3f::Cross3(horizontal, m_direction, m_up);
    horizontal.Normalize();

    // Don't let the model flip upside-down (There is a singularity
    // at the poles when 'up' and 'direction' are aligned)
    float tiltAngle = acos(m_up.Dot3(m_direction));
    if (tiltAngle - ry > 3.13)
        ry = tiltAngle - 3.13;
    else if (tiltAngle - ry < 0.01)
        ry = tiltAngle - 0.01;

    Matrix rotMat = Matrix::MakeAxisRotation(m_up, rx);
    rotMat *= Matrix::MakeAxisRotation(horizontal, ry);

    rotMat.Transform(m_center);
    rotMat.TransformDirection(m_direction);

    // ===========================================
    // ASSIGNMENT 3: Fix any other affected values
    // ===========================================
    m_horizontal = horizontal;
}

// ====================================================================
// ====================================================================

// ====================================================================
// Create a perspective camera with the appropriate dimensions that
// crops or stretches in the x-dimension as necessary
// ====================================================================

void PerspectiveCamera::glInit(int w, int h) {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(m_fov * 180.0 / 3.14159, (float)w / float(h), 0.5, 40.0);
}

// ====================================================================
// Place a perspective camera within an OpenGL scene
// ====================================================================

void PerspectiveCamera::glPlaceCamera(void) {
    gluLookAt(m_center.x(), m_center.y(), m_center.z(),
              m_center.x() + m_direction.x(), m_center.y() + m_direction.y(),
              m_center.z() + m_direction.z(), m_up.x(), m_up.y(), m_up.z());
}

// ====================================================================
// dollyCamera, truckCamera, and RotateCamera
//
// Asumptions:
//  - up is really up (i.e., it hasn't been changed
//    to point to "screen up")
//  - up and direction are normalized
// Special considerations:
//  - If your constructor precomputes any vectors for
//    use in 'generateRay', you will likely to recompute those
//    values at athe end of the these three routines
// ====================================================================

// ====================================================================
// dollyCamera: Move camera along the direction vector
// ====================================================================

void PerspectiveCamera::dollyCamera(float dist) {
    m_center += m_direction * dist;

    // ===========================================
    // ASSIGNMENT 3: Fix any other affected values
    // ===========================================
}

// ====================================================================
// truckCamera: Translate camera perpendicular to the direction vector
// ====================================================================

void PerspectiveCamera::truckCamera(float dx, float dy) {
    Vec3f horizontal;
    Vec3f::Cross3(horizontal, m_direction, m_up);
    horizontal.Normalize();

    Vec3f screenUp;
    Vec3f::Cross3(screenUp, horizontal, m_direction);

    m_center += horizontal * dx + screenUp * dy;

    // ===========================================
    // ASSIGNMENT 3: Fix any other affected values
    // ===========================================
    m_screenup = screenUp;
    m_horizontal = horizontal;
}

// ====================================================================
// rotateCamera: Rotate around the up and horizontal vectors
// ====================================================================

void PerspectiveCamera::rotateCamera(float rx, float ry) {
    Vec3f horizontal;
    Vec3f::Cross3(horizontal, m_direction, m_up);
    horizontal.Normalize();

    // Don't let the model flip upside-down (There is a singularity
    // at the poles when 'up' and 'direction' are aligned)
    float tiltAngle = acos(m_up.Dot3(m_direction));
    if (tiltAngle - ry > 3.13)
        ry = tiltAngle - 3.13;
    else if (tiltAngle - ry < 0.01)
        ry = tiltAngle - 0.01;

    Matrix rotMat = Matrix::MakeAxisRotation(m_up, rx);
    rotMat *= Matrix::MakeAxisRotation(horizontal, ry);

    rotMat.Transform(m_center);
    rotMat.TransformDirection(m_direction);
    m_direction.Normalize();

    // ===========================================
    // ASSIGNMENT 3: Fix any other affected values
    // ===========================================
    // affected: m_screenup
    Vec3f::Cross3(m_screenup, horizontal, m_direction);
    m_screenup.Normalize();
    m_horizontal = horizontal;
}

// ====================================================================
// ====================================================================
