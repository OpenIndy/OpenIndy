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

    bool toOpenIndyXML(QXmlStreamWriter& stream);
    ElementDependencies fromOpenIndyXML(QXmlStreamReader& xml);

    //TODO implement cylinder
};

#endif // CYLINDER_H
