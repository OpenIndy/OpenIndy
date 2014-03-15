#ifndef CONE_H
#define CONE_H

#include "geometry.h"

class Cone : public Geometry
{
public:
    Cone();
    Cone(const Cone &copy);
    void recalc();

    bool toOpenIndyXML(QXmlStreamWriter& stream);
    bool fromOpenIndyXML(QXmlStreamReader& xml);

    //TODO implement cone
};

#endif // CONE_H
