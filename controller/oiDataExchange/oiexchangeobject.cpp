#include "oiexchangeobject.h"

oiExchangeObject::oiExchangeObject(QObject *parent) : QObject(parent){
    device = NULL;
}

oiExchangeObject::oiExchangeObject(const oiExchangeObject &copy, QObject *parent) : QObject(parent){
    this->device = copy.device;
    this->features = copy.features;
    this->unit = copy.unit;
    this->nominalCoordSys = copy.nominalCoordSys;
    this->typeOfElement = copy.typeOfElement;
}

oiExchangeObject &oiExchangeObject::operator=(const oiExchangeObject &copy){
    this->device = copy.device;
    this->features = copy.features;
    this->unit = copy.unit;
    this->nominalCoordSys = copy.nominalCoordSys;
    this->typeOfElement = copy.typeOfElement;
    return *this;
}
