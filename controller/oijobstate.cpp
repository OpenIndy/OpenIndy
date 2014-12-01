#include "oijobstate.h"

OiJobState::OiJobState(QObject *parent) : OiJob(parent)
{
}

const FeatureWrapper *OiJobState::getActiveFeature()
{
    return OiFeatureState::getActiveFeature();
}

const QList<CoordinateSystem *> &OiJobState::getCoordinateSystems()
{
    return OiFeatureState::getCoordinateSystems();
}

const QList<Station *> &OiJobState::getStations()
{
    return OiFeatureState::getStations();
}

const QList<TrafoParam *> &OiJobState::getTrafoParams()
{
    return OiFeatureState::getTransformationParameters();
}

const QList<FeatureWrapper *> &OiJobState::getGeometries()
{
    return OiFeatureState::getGeometries();
}

const Station *OiJobState::getActiveStation()
{
    return OiFeatureState::getActiveStation();
}

const CoordinateSystem *OiJobState::getActiveCoordinateSystem()
{
    return OiFeatureState::getActiveCoordinateSystem();
}
