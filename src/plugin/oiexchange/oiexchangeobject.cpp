#include "oiexchangeobject.h"

OiExchangeObject::OiExchangeObject(QObject *parent) : QObject(parent){
    device = NULL;
}

OiExchangeObject::OiExchangeObject(const OiExchangeObject &copy, QObject *parent) : QObject(parent){
    this->device = copy.device;
    this->features = copy.features;
    this->unit = copy.unit;
    this->nominalCoordSys = copy.nominalCoordSys;
    //this->typeOfElement = copy.typeOfElement;
}

OiExchangeObject &OiExchangeObject::operator=(const OiExchangeObject &copy){
    this->device = copy.device;
    this->features = copy.features;
    this->unit = copy.unit;
    this->nominalCoordSys = copy.nominalCoordSys;
    //this->typeOfElement = copy.typeOfElement;
    return *this;
}
