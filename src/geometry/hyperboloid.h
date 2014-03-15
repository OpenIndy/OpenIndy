#ifndef HYPERBOLOID_H
#define HYPERBOLOID_H

#include "geometry.h"

class Hyperboloid : public Geometry
{
public:
    Hyperboloid();
    Hyperboloid(const Hyperboloid &copy);
    void recalc();

    bool toOpenIndyXML(QXmlStreamWriter& stream);
    bool fromOpenIndyXML(QXmlStreamReader& xml);

    //TODO implement hyperboloid
};

#endif // HYPERBOLOID_H
