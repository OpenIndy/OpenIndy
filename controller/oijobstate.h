#ifndef OIJOBSTATE_H
#define OIJOBSTATE_H

#include "oijob.h"
#include "oifeaturestate.h"

class OiJobState : public OiJob
{
    Q_OBJECT

public:
    OiJobState(QObject *parent = 0);

    const FeatureWrapper* getActiveFeature();
    const QList<CoordinateSystem *>& getCoordinateSystems();
    const QList<Station *>& getStations();
    const QList<TrafoParam *>& getTrafoParams();
    const QList<FeatureWrapper *>& getGeometries();
    const Station* getActiveStation();
    const CoordinateSystem* getActiveCoordinateSystem();

};

#endif // OIJOBSTATE_H
