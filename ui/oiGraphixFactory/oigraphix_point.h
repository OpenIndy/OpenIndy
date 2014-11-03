#ifndef OIGRAPHIX_POINT_H
#define OIGRAPHIX_POINT_H

#include "oigraphix_geometry.h"

class OiGraphixPoint: public OiGraphixGeometry
{
public:
    OiGraphixPoint();

    void draw(Geometry *g);
};

#endif // OIGRAPHIX_POINT_H
