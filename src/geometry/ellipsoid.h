#ifndef ELLIPSOID_H
#define ELLIPSOID_H

#include "geometry.h"

class Ellipsoid : public Geometry
{
public:
    Ellipsoid();
    Ellipsoid(const Ellipsoid &copy);

    OiVec xyz;
    double a,b,c;

    void recalc();

    bool toOpenIndyXML(QXmlStreamWriter& stream);
    ElementDependencies fromOpenIndyXML(QXmlStreamReader& xml);

    //TODO implement ellipsoid
};

#endif // ELLIPSOID_H
