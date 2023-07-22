#include "generator.h"

#include <glincludes.hpp>

void RingGenerator::Paint() const {
    glDisable(GL_LIGHTING);
    glBegin(GL_QUADS);
    glColor3f(0.3f, 0.3f, 0.3f);
    float size = 10.0f;
    glVertex3f(-size, offsetY, -size);
    glVertex3f(-size, offsetY, size);
    glVertex3f(size, offsetY, size);
    glVertex3f(size, offsetY, -size);
    glEnd();
    glEnable(GL_LIGHTING);
}