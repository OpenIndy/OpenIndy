#ifndef MEASUREMENTCONFIG_H
#define MEASUREMENTCONFIG_H

#include <QtXml>

#include "configuration.h"

class OiConfigState;

/*!
 * \brief The MeasurementConfig class
 * contains all the configuration parameters needed to start a measurement.
 */
class MeasurementConfig
{
    friend class OiConfigState;

public:
    MeasurementConfig();

    //! compare the attributes of both measurement configs
    friend bool operator==(const MeasurementConfig &left, const MeasurementConfig &right){

        if(left.count == right.count && left.iterations == right.iterations
                && left.measureTwoSides == right.measureTwoSides
                && left.timeDependent == right.timeDependent && left.distanceDependent == right.distanceDependent
                && left.timeInterval == right.timeInterval && left.distanceInterval == right.distanceInterval
                && left.typeOfReading == right.typeOfReading){
            return true;
        }else{
            return false;
        }

    }

    QString getName() const;
    QString getDisplayName() const;
    bool getIsSaved() const;
    bool getIsValid() const;

    bool setName(QString name);

    //measurement config parameters
    int count; //the number of measurements that the sensor shall bring together to a single reading
    int iterations; //the number of readings the sensor shall return
    bool measureTwoSides; //true if the sensor shall measure in both faces (false if not)
    bool timeDependent; //true if the sensor shall measure in a special timeInterval (false if not)
    bool distanceDependent; //true if the sensor shall measure in a special distanceInterval (false if not)
    long timeInterval; //time interval in which the sensor shall measure
    double distanceInterval; //distance interval in which the sensor shall measure
    Configuration::ReadingTypes typeOfReading; //the type of reading which the sensor shall return

    //xml import export
    QDomElement toOpenIndyXML(QDomDocument &xmlDoc) const;
    bool fromOpenIndyXML(QDomElement &xmlElem);

private:
    //name of the config
    QString name;

    //true if the config is saved (reusable when restarting OpenIndy), false if not
    bool isSaved;

    //only OiConfigState can access this method from outside this class
    void setIsSaved(bool isSaved);
};

#endif // MEASUREMENTCONFIG_H
