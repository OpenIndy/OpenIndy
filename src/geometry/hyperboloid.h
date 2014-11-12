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
    bool fromOpenIndyXML(QDomElement &xmlElem);

    bool saveSimulationData();

    static MeasurementConfig defaultMeasurementConfig; //used as a default for this geometry type

    //TODO implement hyperboloid
};

#endif // HYPERBOLOID_H
