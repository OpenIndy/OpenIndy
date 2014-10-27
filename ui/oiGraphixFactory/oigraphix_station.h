#ifndef OIGRAPHIX_STATION_H
#define OIGRAPHIX_STATION_H

#include "oigraphix_geometry.h"

class OiGraphixStation: public OiGraphixGeometry
{
public:
    OiGraphixStation();

    void draw(Geometry *g);
};

#endif // OIGRAPHIX_STATION_H
