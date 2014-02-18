#ifndef OIGRAPHIX_POINT_H
#define OIGRAPHIX_POINT_H

#include "oigraphix_geometry.h"

class OiGraphixPoint: public OiGraphixGeometry
{
public:
    OiGraphixPoint();

    void draw(GLfloat x, GLfloat y, GLfloat z);
};

#endif // OIGRAPHIX_POINT_H
