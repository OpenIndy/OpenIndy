#ifndef CIRCLE_H
#define CIRCLE_H

#include "geometry.h"

class Circle : public Geometry
{
public:
    Circle();
    Circle(const Circle &copy);
    void recalc();

    //TODO implement circle
};

#endif // CIRCLE_H
