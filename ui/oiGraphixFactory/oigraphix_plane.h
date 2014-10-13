#ifndef OIGRAPHIX_PLANE_H
#define OIGRAPHIX_PLANE_H

#include "oigraphix_geometry.h"

class OiGraphixPlane: public OiGraphixGeometry
{
public:
    OiGraphixPlane();

    GLfloat rx;
    GLfloat ry;
    GLfloat rz;

    void draw(Geometry *g);
};

#endif // OIGRAPHIX_PLANE_H
