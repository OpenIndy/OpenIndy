#ifndef OIGRAPHIX_LINE_H
#define OIGRAPHIX_LINE_H

#include "oigraphix_geometry.h"

class OiGraphixLine: public OiGraphixGeometry
{
public:
    OiGraphixLine();

    GLfloat rx;
    GLfloat ry;
    GLfloat rz;

    void draw(Geometry *g);
};

#endif // OIGRAPHIX_LINE_H
