#include "oiexchangeobject.h"

OiExchangeObject::OiExchangeObject(QObject *parent) : QObject(parent){
    this->device = NULL;
}

OiExchangeObject::OiExchangeObject(const OiExchangeObject &copy, QObject *parent) : QObject(parent){
    this->device = copy.device;
    this->features = copy.features;
}

OiExchangeObject &OiExchangeObject::operator=(const OiExchangeObject &copy){
    this->device = copy.device;
    this->features = copy.features;
    return *this;
}
