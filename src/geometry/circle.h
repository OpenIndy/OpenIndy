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


    bool toOpenIndyXML(QXmlStreamWriter& stream);
    ElementDependencies fromOpenIndyXML(QXmlStreamReader& xml);

};

#endif // CIRCLE_H
