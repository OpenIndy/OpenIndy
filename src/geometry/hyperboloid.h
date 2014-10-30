#ifndef HYPERBOLOID_H
#define HYPERBOLOID_H

#include "geometry.h"

class Hyperboloid : public Geometry
{
    Q_OBJECT
public:
    Hyperboloid(bool isNominal, QObject *parent = 0);
    Hyperboloid(const Hyperboloid &copy);
    void recalc();

    QDomElement toOpenIndyXML(QDomDocument &xmlDoc) const;
    ElementDependencies fromOpenIndyXML(QXmlStreamReader& xml);

    bool saveSimulationData();

    //TODO implement hyperboloid
};

#endif // HYPERBOLOID_H
