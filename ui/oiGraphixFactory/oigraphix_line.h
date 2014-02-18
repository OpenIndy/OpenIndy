#ifndef OIGRAPHIX_LINE_H
#define OIGRAPHIX_LINE_H

#include "oigraphix_geometry.h"

class OiGraphixLine: public OiGraphixGeometry
{
public:
    OiGraphixLine(GLfloat i, GLfloat j, GLfloat k);

    GLfloat rx;
    GLfloat ry;
    GLfloat rz;

    void draw(GLfloat x, GLfloat y, GLfloat z);
};

#endif // OIGRAPHIX_LINE_H
