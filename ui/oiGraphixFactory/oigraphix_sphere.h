#ifndef OIGRAPHIX_SPHERE_H
#define OIGRAPHIX_SPHERE_H

#include "oigraphix_geometry.h"


class OiGraphixSphere : public OiGraphixGeometry
{
public:
    OiGraphixSphere(float radius, unsigned int rings, unsigned int sectors);

    void draw(GLfloat x, GLfloat y, GLfloat z);

private:
    GLfloat radius;
};

#endif // OIGRAPHIX_SPHERE_H
