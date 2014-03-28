#ifndef CONE_H
#define CONE_H

#include "geometry.h"

class Cone : public Geometry
{
public:
    Cone();
    Cone(const Cone &copy);

    OiVec xyz;
    OiVec ijk;
    double alpha;

    void recalc();

    bool toOpenIndyXML(QXmlStreamWriter& stream);
    ElementDependencies fromOpenIndyXML(QXmlStreamReader& xml);

    //TODO implement cone
};

#endif // CONE_H
