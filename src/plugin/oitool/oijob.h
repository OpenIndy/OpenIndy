#ifndef OIJOB_H
#define OIJOB_H

#include <QObject>
#include "featurewrapper.h"
#include "coordinatesystem.h"
#include "station.h"
#include "trafoparam.h"

class OiJob : public QObject
{
    Q_OBJECT

public:
    explicit OiJob(QObject *parent = 0);

    virtual const FeatureWrapper* getActiveFeature() = 0;
    virtual const QList<CoordinateSystem *>& getCoordinateSystems() = 0;
    virtual const QList<Station *>& getStations() = 0;
    virtual const QList<TrafoParam *>& getTrafoParams() = 0;
    virtual const QList<FeatureWrapper *> &getGeometries() = 0;
    virtual const Station* getActiveStation() = 0;
    virtual const CoordinateSystem *getActiveCoordinateSystem() = 0;

signals:
    void activeFeatureChanged();
    void activeStationChanged();
    void activeCoordinateSystemChanged();
    void featureSetChanged();
    void featureAttributesChanged();
    void geometryObservationsChanged();
    void featureFunctionsChanged();
    void coordSystemSetChanged();

    void startAim();
    void showWatchWindow();

public slots:
    void emitActiveFeatureChanged();
    void emitActiveStationChanged();
    void emitActiveCoordinateSystemChanged();
    void emitFeatureSetChanged();
    void emitFeatureAttributesChanged();
    void emitGeometryObservationsChanged();
    void emitFeatureFunctionsChanged();
    void emitCoordSystemSetChanged();

};

#endif // OIJOB_H
