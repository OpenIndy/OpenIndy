#ifndef OIGRAPHIX_PLANE_H
#define OIGRAPHIX_PLANE_H

#include "oigraphix_geometry.h"

class OiGraphixPlane: public OiGraphixGeometry
{
public:
    OiGraphixPlane(GLfloat nx,GLfloat ny, GLfloat nz);

    GLfloat rx;
    GLfloat ry;
    GLfloat rz;

    void draw(GLfloat x, GLfloat y, GLfloat z);
};

#endif // OIGRAPHIX_PLANE_H
