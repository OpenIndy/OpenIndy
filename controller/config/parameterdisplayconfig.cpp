#include "parameterdisplayconfig.h"

/*!
 * \brief ParameterDisplayConfig::ParameterDisplayConfig
 */
ParameterDisplayConfig::ParameterDisplayConfig(){

    //load default parameter display config
    this->init();

}

/*!
 * \brief ParameterDisplayConfig::getDisplayUnit
 * \param dimension
 * \return
 */
UnitType ParameterDisplayConfig::getDisplayUnit(const DimensionType &dimension) const{
    switch(dimension){
    case eMetric:
        return this->metricDisplayUnit;
    case eAngular:
        return this->angularDisplayUnit;
    case eTemperature:
        return this->temperatureDisplayUnit;
    case eDimensionless:
        return eNoUnit;
    }
}

/*!
 * \brief ParameterDisplayConfig::setDisplayUnitType
 * \param dimension
 * \param unit
 */
void ParameterDisplayConfig::setDisplayUnitType(const DimensionType &dimension, const UnitType &unit){
    switch(dimension){
    case eMetric:
        if(unit == eUnitMeter || unit == eUnitMilliMeter || unit == eUnitInch){
            this->metricDisplayUnit = unit;
        }
        break;
    case eAngular:
        if(unit == eUnitRadiant || unit == eUnitMilliRadians || unit == eUnitGon || unit == eUnitDecimalDegree || unit == eUnitArcSeconds){
            this->angularDisplayUnit = unit;
        }
        break;
    case eTemperature:
        if(unit == eUnitGrad || unit == eUnitKelvin || unit == eUnitFahrenheit){
            this->temperatureDisplayUnit = unit;
        }
        break;
    case eDimensionless:
        break;
    }
}

/*!
 * \brief ParameterDisplayConfig::getDisplayDigits
 * \param dimension
 * \return
 */
int ParameterDisplayConfig::getDisplayDigits(const DimensionType &dimension) const{
    switch(dimension){
    case eMetric:
        return this->metricDisplayDigits;
    case eAngular:
        return this->angularDisplayDigits;
    case eTemperature:
        return this->temperatureDisplayDigits;
    case eDimensionless:
        return this->dimensionlessDisplayDigits;
    }
}

/*!
 * \brief ParameterDisplayConfig::setDisplayDigits
 * \param dimension
 * \param digits
 */
void ParameterDisplayConfig::setDisplayDigits(const DimensionType &dimension, const int &digits){

    //check digits
    if(digits < 0){
        return;
    }

    //set digits
    switch(dimension){
    case eMetric:
        this->metricDisplayDigits = digits;
        break;
    case eAngular:
        this->angularDisplayDigits = digits;
        break;
    case eTemperature:
        this->temperatureDisplayDigits = digits;
        break;
    case eDimensionless:
        this->dimensionlessDisplayDigits = digits;
        break;
    }

}

/*!
 * \brief ParameterDisplayConfig::init
 * Set up default parameter display config
 */
void ParameterDisplayConfig::init(){

    //default units
    this->metricDisplayUnit = eUnitMeter;
    this->angularDisplayUnit = eUnitDecimalDegree;
    this->temperatureDisplayUnit = eUnitGrad;

    //default digits
    this->metricDisplayDigits = 4;
    this->angularDisplayDigits = 6;
    this->temperatureDisplayDigits = 2;
    this->dimensionlessDisplayDigits = 6;

}
