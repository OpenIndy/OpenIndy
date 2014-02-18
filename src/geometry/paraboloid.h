#ifndef PARABOLOID_H
#define PARABOLOID_H

#include "geometry.h"

class Paraboloid : public Geometry
{
public:
    Paraboloid();
    Paraboloid(const Paraboloid &copy);
    void recalc();

    //TODO implement paraboloid
};

#endif // PARABOLOID_H
