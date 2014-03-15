#ifndef NURBS_H
#define NURBS_H

#include "geometry.h"

class Nurbs : public Geometry
{
public:
    Nurbs();
    Nurbs(const Nurbs &copy);
    void recalc();

    bool toOpenIndyXML(QXmlStreamWriter& stream);
    bool fromOpenIndyXML(QXmlStreamReader& xml);

    //TODO implement nurbs
};

#endif // NURBS_H
