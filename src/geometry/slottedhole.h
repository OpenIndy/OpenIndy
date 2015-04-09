#ifndef SLOTTEDHOLE_H
#define SLOTTEDHOLE_H

#include "geometry.h"

class SlottedHole : public Geometry
{
    Q_OBJECT
public:
    SlottedHole(bool isNominal, QObject *parent = 0);
    SlottedHole(const SlottedHole &copy);
    void recalc();

    QDomElement toOpenIndyXML(QDomDocument &xmlDoc) const;
    bool fromOpenIndyXML(QDomElement &xmlElem);

    bool saveSimulationData();

    static MeasurementConfig defaultMeasurementConfig; //used as a default for this geometry type

    //TODO implement paraboloid
};

#endif // SLOTTEDHOLE_H
