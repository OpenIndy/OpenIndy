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
        unknownR
    };

    Circle();
    Circle(const Circle &copy);

    OiVec xyz;
    double radius;

    void recalc();

};

#endif // CIRCLE_H
