#include "oijob.h"

OiJob *OiJob::jobInstance = NULL;

FeatureWrapper *OiJob::myActiveFeature = NULL;
QList<CoordinateSystem *> OiJob::myCoordinateSystems;
QList<Station *> OiJob::myStations;
QList<TrafoParam *> OiJob::myTransformationParameters;
QList<FeatureWrapper *> OiJob::myGeometries;
Station *OiJob::myActiveStation = NULL;
CoordinateSystem *OiJob::myActiveCoordinateSystem = NULL;

OiJob::OiJob(QObject *parent) :
    QObject(parent)
{
}

OiJob *OiJob::getInstance()
{
    if(OiJob::jobInstance == NULL){
        OiJob::jobInstance = new OiJob();
    }
    return OiJob::jobInstance;
}

const QList<CoordinateSystem *> &OiJob::getCoordinateSystems()
{
    return OiJob::myCoordinateSystems;
}

const QList<Station *> &OiJob::getStations()
{
    return OiJob::myStations;
}

const QList<FeatureWrapper *> &OiJob::getGeometries()
{
    return OiJob::myGeometries;
}

const Station *OiJob::getActiveStation()
{
    return OiJob::myActiveStation;
}

const CoordinateSystem *OiJob::getActiveCoordinateSystem()
{
    return OiJob::myActiveCoordinateSystem;
}

const QList<TrafoParam *> &OiJob::getTrafoParams()
{
    return OiJob::myTransformationParameters;
}

const FeatureWrapper *OiJob::getActiveFeature()
{
    return OiJob::myActiveFeature;
}

void OiJob::emitActiveFeatureChanged()
{
    emit this->activeFeatureChanged();
}

void OiJob::emitActiveStationChanged()
{
    emit this->activeStationChanged();
}

void OiJob::emitActiveCoordinateSystemChanged()
{
    emit this->activeCoordinateSystemChanged();
}

void OiJob::emitFeatureSetChanged()
{
    emit this->featureSetChanged();
}

void OiJob::emitFeatureAttributesChanged()
{
    emit this->featureAttributesChanged();
}

void OiJob::emitGeometryObservationsChanged()
{
    emit this->geometryObservationsChanged();
}

void OiJob::emitFeatureFunctionsChanged()
{
    emit this->featureAttributesChanged();
}

void OiJob::emitCoordSystemSetChanged()
{
    emit this->coordSystemSetChanged();
}

