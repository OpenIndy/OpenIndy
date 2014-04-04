#ifndef CIRCLE_H
#define CIRCLE_H

#include "geometry.h"

class Circle : public Geometry
{
public:
    enum CircleUnknowns{
        unknownX,
        unknownY,
        unknownZ,
        unknownI,
        unknownJ,
        unknownK,
        unknownR,
        unknownD
    };

    Circle();
    Circle(const Circle &copy);

    OiVec xyz;
    OiVec ijk;
    double radius;
    double dist2origin;

    void recalc();

};

#endif // CIRCLE_H
