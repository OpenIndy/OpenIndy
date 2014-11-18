#ifndef ELLIPSOID_H
#define ELLIPSOID_H

#include "geometry.h"

class Ellipsoid : public Geometry
{
    Q_OBJECT
public:
    Ellipsoid(bool isNominal, QObject *parent = 0);
    Ellipsoid(const Ellipsoid &copy);

    OiVec xyz;
    double a,b,c;
    OiVec getXYZ() const;

    void recalc();

    QDomElement toOpenIndyXML(QDomDocument &xmlDoc) const;
    bool fromOpenIndyXML(QDomElement &xmlElem);

    bool saveSimulationData();

    static MeasurementConfig defaultMeasurementConfig; //used as a default for this geometry type

    //TODO implement ellipsoid
};

#endif // ELLIPSOID_H
