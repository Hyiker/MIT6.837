#ifndef ASSIGNMENT1_MATERIAL_HPP
#define ASSIGNMENT1_MATERIAL_HPP

#include "vectors.h"

// ====================================================================
// ====================================================================

// You will extend this class in later assignments

class Material {
   public:
    // CONSTRUCTORS & DESTRUCTOR
    Material(const Vec3f &d_color) { diffuseColor = d_color; }
    virtual ~Material() {}

    // ACCESSORS
    virtual Vec3f getDiffuseColor() const { return diffuseColor; }

   protected:
    // REPRESENTATION
    Vec3f diffuseColor;
};

// ====================================================================
// ====================================================================

#endif /* ASSIGNMENT1_MATERIAL_HPP */
