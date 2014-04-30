#ifndef FEATUREATTRIBUTESEXCHANGE_H
#define FEATUREATTRIBUTESEXCHANGE_H

#include <QString>
#include "configuration.h"
#include "coordinatesystem.h"

class FeatureAttributesExchange
{
public:
    FeatureAttributesExchange();
    FeatureAttributesExchange(int count, int featureType, QString name, QString group,
                              bool actual, bool nominal, bool common, CoordinateSystem *nominalSystem);
    FeatureAttributesExchange(int count, int featureType, QString name, QString group,
                              bool actual, bool nominal, bool common, CoordinateSystem *nominalSystem,
                              CoordinateSystem *startSystem, CoordinateSystem *destSystem);

    int count;
    int featureType;
    QString name;
    QString group;
    bool actual;
    bool nominal;
    bool common;
    CoordinateSystem *nominalSystem;
    CoordinateSystem *startSystem;
    CoordinateSystem *destSystem;
};

#endif // FEATUREATTRIBUTESEXCHANGE_H
