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

    friend bool operator==(const MeasurementConfig &left, const MeasurementConfig &right){
        if(left.name.compare(right.name) == 0 && left.internalRef == right.internalRef){
            return true;
        }else{
            return false;
        }
    }

    QString name;
    int count;
    int iterations;
    bool measureTwoSides;
    bool timeDependent;
    bool distanceDependent;
    long timeInterval;
    double distanceInterval;
    Configuration::ReadingTypes typeOfReading;

    //xml import export
    bool toOpenIndyXML(QXmlStreamWriter& stream);
    ElementDependencies fromOpenIndyXML(QXmlStreamReader& xml);

private:
    int internalRef; //internalRef to distinguish between different measurement configs with the same name
    bool isSaved; //true if the config is saved (reusable when restarting OpenIndy), false if not
};

#endif // MEASUREMENTCONFIG_H
