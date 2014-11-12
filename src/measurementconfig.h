#ifndef MEASUREMENTCONFIG_H
#define MEASUREMENTCONFIG_H

#include <QtXml>

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

    //! compare the attributes of both measurement configs
    friend bool operator==(const MeasurementConfig &left, const MeasurementConfig &right){

        if(left.count == right.count && left.iterations == right.iterations
                && left.face == right.face && left.measureTwoSides == right.measureTwoSides
                && left.timeDependent == right.timeDependent && left.distanceDependent == right.distanceDependent
                && left.timeInterval == right.timeInterval && left.distanceInterval == right.distanceInterval
                && left.typeOfReading == right.typeOfReading){
            return true;
        }else{
            return false;
        }

    }

    //name of the config (always unique)
    QString name;

    //measurement config parameters
    int count;
    int iterations;
    Configuration::Faces face;
    bool measureTwoSides;
    bool timeDependent;
    bool distanceDependent;
    long timeInterval;
    double distanceInterval;
    Configuration::ReadingTypes typeOfReading;

    //true if the config is saved (reusable when restarting OpenIndy), false if not
    bool isSaved;

    //xml import export
    QDomElement toOpenIndyXML(QDomDocument &xmlDoc) const;
    bool fromOpenIndyXML(QDomElement &xmlElem);
};

#endif // MEASUREMENTCONFIG_H
