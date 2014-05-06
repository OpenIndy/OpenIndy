#include "featureattributesexchange.h"

/*!
 * \brief FeatureAttributesExchange is a class to exchange the values of attributes of features.
 * Using this class avoids a big number of input parameters of functions. Instead use only this class als input
 * parameter and fill its attributes.
 */
FeatureAttributesExchange::FeatureAttributesExchange()
{
    this->count = 0;
    this->featureType = -1;
    this->name = "";
    this->group = "";
    this->actual = false;
    this->nominal = false;
    this->common = false;
    this->nominalSystem = NULL;
    this->startSystem = NULL;
    this->destSystem = NULL;
}

FeatureAttributesExchange::FeatureAttributesExchange(int count, int featureType, QString name, QString group, bool actual, bool nominal, bool common, CoordinateSystem *nominalSystem)
{
    this->count = count;
    this->featureType = featureType;
    this->name = name;
    this->group = group;
    this->actual = actual;
    this->nominal = nominal;
    this->common = common;
    this->nominalSystem = nominalSystem;
    this->startSystem = NULL;
    this->destSystem = NULL;
}

FeatureAttributesExchange::FeatureAttributesExchange(int count, int featureType, QString name, QString group, bool actual, bool nominal, bool common, CoordinateSystem *nominalSystem, CoordinateSystem *startSystem, CoordinateSystem *destSystem)
{
    this->count = count;
    this->featureType = featureType;
    this->name = name;
    this->group = group;
    this->actual = actual;
    this->nominal = nominal;
    this->common = common;
    this->nominalSystem = nominalSystem;
    this->startSystem = startSystem;
    this->destSystem = destSystem;
}

