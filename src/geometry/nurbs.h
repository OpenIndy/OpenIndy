#ifndef NURBS_H
#define NURBS_H

#include "geometry.h"

class Nurbs : public Geometry
{
    Q_OBJECT
public:
    Nurbs(bool isNominal, QObject *parent = 0);
    Nurbs(const Nurbs &copy);
    void recalc();

    bool toOpenIndyXML(QXmlStreamWriter& stream);
    ElementDependencies fromOpenIndyXML(QXmlStreamReader& xml);

    //TODO implement nurbs
};

#endif // NURBS_H
