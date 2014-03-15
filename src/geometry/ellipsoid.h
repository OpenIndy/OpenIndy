#ifndef ELLIPSOID_H
#define ELLIPSOID_H

#include "geometry.h"

class Ellipsoid : public Geometry
{
public:
    Ellipsoid();
    Ellipsoid(const Ellipsoid &copy);
    void recalc();

    bool toOpenIndyXML(QXmlStreamWriter& stream);
    bool fromOpenIndyXML(QXmlStreamReader& xml);

    //TODO implement ellipsoid
};

#endif // ELLIPSOID_H
