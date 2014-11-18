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

    QDomElement toOpenIndyXML(QDomDocument &xmlDoc) const;
    bool fromOpenIndyXML(QDomElement &xmlElem);

    bool saveSimulationData();

    static MeasurementConfig defaultMeasurementConfig; //used as a default for this geometry type

    //TODO implement nurbs
};

#endif // NURBS_H
