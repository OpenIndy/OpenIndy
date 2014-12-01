#include "oijob.h"

OiJob::OiJob(QObject *parent) :
    QObject(parent)
{
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

