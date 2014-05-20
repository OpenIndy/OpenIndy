#ifndef MEASUREMENTCONFIG_H
#define MEASUREMENTCONFIG_H

#include <QXmlStreamWriter>
#include <QXmlStreamReader>

#include "configuration.h"
#include "elementdependencies.h"


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
    int iterations;
    Configuration::Faces face;
    bool measureTwoSides;
    bool timeDependent;
    bool distanceDependent;

    long timeInterval;
    double distanceInterval;

    //reading
    Configuration::ReadingTypes typeOfReading;

    //xml import export
    bool toOpenIndyXML(QXmlStreamWriter& stream);
    ElementDependencies fromOpenIndyXML(QXmlStreamReader& xml);
};

#endif // MEASUREMENTCONFIG_H
