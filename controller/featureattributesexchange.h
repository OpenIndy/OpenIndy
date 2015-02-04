#ifndef FEATUREATTRIBUTESEXCHANGE_H
#define FEATUREATTRIBUTESEXCHANGE_H

#include <QString>
#include "configuration.h"
#include "coordinatesystem.h"

class FeatureAttributes
{
public:
    FeatureAttributes();

    int count;
    Configuration::FeatureTypes featureType;
    QString name;
    QString group;
    bool actual;
    bool nominal;
    bool common;
    CoordinateSystem *nominalSystem;
    CoordinateSystem *startSystem;
    CoordinateSystem *destSystem;

    QString plugin;
    QString function;

    //trafo param
    bool isMovement;
};

#endif // FEATUREATTRIBUTESEXCHANGE_H
