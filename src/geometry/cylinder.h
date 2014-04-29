#ifndef CYLINDER_H
#define CYLINDER_H

#include "geometry.h"

class Cylinder : public Geometry
{
public:
    Cylinder();
    Cylinder(const Cylinder &copy);

    OiVec xyz;
    OiVec ijk;
    double radius;

    void recalc();

    //TODO implement cylinder
};

#endif // CYLINDER_H
