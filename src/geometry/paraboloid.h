#ifndef PARABOLOID_H
#define PARABOLOID_H

#include "geometry.h"

class Paraboloid : public Geometry
{
    Q_OBJECT
public:
    Paraboloid(bool isNominal, QObject *parent = 0);
    Paraboloid(const Paraboloid &copy);
    void recalc();

    QDomElement toOpenIndyXML(QDomDocument &xmlDoc);
    ElementDependencies fromOpenIndyXML(QXmlStreamReader& xml);

    bool saveSimulationData();

    //TODO implement paraboloid
};

#endif // PARABOLOID_H
