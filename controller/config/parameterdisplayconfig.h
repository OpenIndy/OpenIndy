#ifndef PARAMETERDISPLAYCONFIG_H
#define PARAMETERDISPLAYCONFIG_H

#include "types.h"

using namespace oi;

/*!
 * \brief The ParameterDisplayConfig class
 * Used to specify in which unit and with how many digits a parameter value shall be displayed
 */
class ParameterDisplayConfig
{
public:
    ParameterDisplayConfig();

    UnitType getDisplayUnit(const DimensionType &dimension) const;
    void setDisplayUnitType(const DimensionType &dimension, const UnitType &unit);

    int getDisplayDigits(const DimensionType &dimension) const;
    void setDisplayDigits(const DimensionType &dimension, const int &digits);

private:
    void init();

    UnitType metricDisplayUnit; //unit in which metric parameter values are displayed
    UnitType angularDisplayUnit; //unit in which angular parameter values are displayed
    UnitType temperatureDisplayUnit; //unit in which temperature parameter values are displayed

    int metricDisplayDigits; //number of digits that are displayed for metric parameter values
    int angularDisplayDigits; //number of digits that are displayed for angular parameter values
    int temperatureDisplayDigits; //number of digits that are displayed for temperature parameter values
    int dimensionlessDisplayDigits; //number of digits that are displayed for dimensionless parameter values
};

#endif // PARAMETERDISPLAYCONFIG_H
