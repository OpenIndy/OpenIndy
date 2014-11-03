#ifndef OIGRAPHIX_SPHERE_H
#define OIGRAPHIX_SPHERE_H

#include "oigraphix_geometry.h"


class OiGraphixSphere : public OiGraphixGeometry
{
public:
    OiGraphixSphere();

    void draw(Geometry *g);

private:
    GLfloat radius;

    GLfloat rx;
    GLfloat ry;
    GLfloat rz;
};

#endif // OIGRAPHIX_SPHERE_H
