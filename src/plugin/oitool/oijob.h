#ifndef OIJOB_H
#define OIJOB_H

#include <QObject>
#include "featurewrapper.h"
#include "coordinatesystem.h"
#include "station.h"
#include "trafoparam.h"

class OiFeatureState;

class OiJob : public QObject
{
    Q_OBJECT

    friend class OiFeatureState;

private:
    explicit OiJob(QObject *parent = 0);
    static OiJob *jobInstance;

public:
    static OiJob *getInstance();

    static const FeatureWrapper* getActiveFeature();
    static const QList<CoordinateSystem *>& getCoordinateSystems();
    static const QList<Station *>& getStations();
    static const QList<TrafoParam *>& getTrafoParams();
    static const QList<FeatureWrapper *>& getGeometries();
    static const Station* getActiveStation();
    static const CoordinateSystem* getActiveCoordinateSystem();

signals:
    void activeFeatureChanged();
    void activeStationChanged();
    void activeCoordinateSystemChanged();
    void featureSetChanged();
    void featureAttributesChanged();
    void geometryObservationsChanged();
    void featureFunctionsChanged();
    void coordSystemSetChanged();

public slots:

    void emitActiveFeatureChanged();
    void emitActiveStationChanged();
    void emitActiveCoordinateSystemChanged();
    void emitFeatureSetChanged();
    void emitFeatureAttributesChanged();
    void emitGeometryObservationsChanged();
    void emitFeatureFunctionsChanged();
    void emitCoordSystemSetChanged();

private:
    static FeatureWrapper *myActiveFeature; //the currently selected feature in table view
    static QList<CoordinateSystem *> myCoordinateSystems; //list of all non-station coordinate systems
    static QList<Station *> myStations; //list of all stations (each station has pointer to its coordinate system)
    static QList<TrafoParam *> myTransformationParameters; //list of all trafo params
    static QList<FeatureWrapper *> myGeometries; //list of all geometry features
    static Station *myActiveStation; //the currently selected station
    static CoordinateSystem *myActiveCoordinateSystem; //the currently selected display coordinate system

};

#endif // OIJOB_H
