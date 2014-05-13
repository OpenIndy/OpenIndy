#include "featureattributesexchange.h"

/*!
 * \brief FeatureAttributesExchange is a class to exchange the values of attributes of features.
 * Using this class avoids a big number of input parameters of functions. Instead use only this class als input
 * parameter and fill its attributes.
 */
FeatureAttributesExchange::FeatureAttributesExchange()
{
    this->count = 0;
    this->name = "";
    this->group = "";
    this->actual = false;
    this->nominal = false;
    this->common = false;
    this->nominalSystem = NULL;
    this->startSystem = NULL;
    this->destSystem = NULL;
    this->function = "";
}

FeatureAttributesExchange::FeatureAttributesExchange(int count, Configuration::FeatureTypes featureType, QString name, QString group, QString function, bool actual, bool nominal, bool common, CoordinateSystem *nominalSystem)
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
    this->function = function;
}

FeatureAttributesExchange::FeatureAttributesExchange(int count, Configuration::FeatureTypes featureType, QString name, QString group, QString function, bool actual, bool nominal, bool common, CoordinateSystem *nominalSystem, CoordinateSystem *startSystem, CoordinateSystem *destSystem)
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
    this->function = function;
}

