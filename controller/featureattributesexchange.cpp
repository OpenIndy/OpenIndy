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
    this->isMovement = false;
}

