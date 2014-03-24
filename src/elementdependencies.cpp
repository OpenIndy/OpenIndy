#include "elementdependencies.h"

ElementDependencies::ElementDependencies()
{
    this->hasSensorPlugin = false;
    this->hasFunctionPlugin = false;
    this->hasObservations = false;
    this->hasFeatures = false;
}

bool ElementDependencies::addFeatureID(int id,QString featureGroup){

    this->hasFeatures = true;

    if(!this->features.contains(featureGroup)){
        QList<int> *l = new QList<int>;
        this->features.insert(featureGroup,l);
    }

    this->features.value("featureGroup")->append(id);

    return true;
}


bool ElementDependencies::addObservationID(int id){

    this->hasObservations = true;

    this->observations.append(id);

    return true;
}

bool ElementDependencies::addFunctionInfo(functionInfo info){

    this->hasFunctionPlugin = true;

    this->neededFunctions.append(info);

    return true;
}

bool ElementDependencies::addSensorInfo(sensorInfo info){

    this->hasSensorPlugin = true;

    this->neededSensors.append(info);

    return true;
}
