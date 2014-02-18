#ifndef MEASUREMENTCONFIG_H
#define MEASUREMENTCONFIG_H

#include "configuration.h"

/*!
 * \brief The MeasurementConfig class
 * contains all the configuration parameters needed to start a measurement.
 */
class MeasurementConfig
{
public:
    MeasurementConfig();

    QString name;
    int count;
    Configuration::Faces face;
    bool measureTwoSides;
    bool timeDependent;
    bool distanceDependent;

    long timeInterval;
    double distanceInterval;

    //reading
    Configuration::ReadingTypes typeOfReading;
};

#endif // MEASUREMENTCONFIG_H
