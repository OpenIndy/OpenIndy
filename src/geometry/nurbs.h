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

    QDomElement toOpenIndyXML(QDomDocument &xmlDoc);
    ElementDependencies fromOpenIndyXML(QXmlStreamReader& xml);

    bool saveSimulationData();

    //TODO implement nurbs
};

#endif // NURBS_H
