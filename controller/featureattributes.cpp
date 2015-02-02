#include "featureattributes.h"

/*!
 * \brief FeatureAttributesExchange is a class to exchange the values of attributes of features.
 * Using this class avoids a big number of input parameters of functions. Instead use only this class als input
 * parameter and fill its attributes.
 */
FeatureAttributes::FeatureAttributes()
{
    this->count = 0;
    this->isActual = false;
    this->isNominal = false;
    this->isCommon = false;
    this->isMovement = false;
}

