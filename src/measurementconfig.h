#ifndef MEASUREMENTCONFIG_H
#define MEASUREMENTCONFIG_H

#include <QString>
#include <QtXml>

#include "types.h"

//class OiConfigState;

/*!
 * \brief The MeasurementConfig class
 * Contains all the configuration parameters needed to start a measurement.
 */
class MeasurementConfig
{
    //friend class OiConfigState;

public:
    MeasurementConfig();
/*
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

    }*/

    //########################################
    //get or set measurement config attributes
    //########################################

    const QString &getName() const;
    void setName(const QString &name);

    const bool &getIsSaved() const;

    bool getIsValid() const;

    const int &getCount() const;
    void setCount(const int &count);

    const int &getIterations() const;
    void setIterations(const int &iterations);

    const bool &getMeasureTwoSides() const;
    void setMeasureTwoSides(const bool &twoSides);

    const bool &getTimeDependent() const;
    void setTimeDependent(const bool &timeDependent);

    const bool &getDistanceDependent() const;
    void setDistanceDependent(const bool &distanceDependent);

    const long &getTimeInterval() const;
    void setTimeInterval(const long &interval);

    const double &getDistanceInterval() const;
    void setDistanceInterval(const double &interval);

    const ReadingTypes &getTypeOfReading() const;
    void setTypeOfReading(const ReadingTypes &type);

    //#################
    //save and load XML
    //#################

    QDomElement toOpenIndyXML(QDomDocument &xmlDoc) const;
    bool fromOpenIndyXML(QDomElement &xmlElem);

private:

    //#############################
    //measurement config attributes
    //#############################

    QString name;

    bool isSaved;

    int count; //the number of measurements that the sensor shall bring together to a single reading
    int iterations; //the number of readings the sensor shall return
    bool measureTwoSides; //true if the sensor shall measure in both faces (false if not)
    bool timeDependent; //true if the sensor shall measure in a special timeInterval (false if not)
    bool distanceDependent; //true if the sensor shall measure in a special distanceInterval (false if not)
    long timeInterval; //time interval in which the sensor shall measure
    double distanceInterval; //distance interval in which the sensor shall measure
    ReadingTypes typeOfReading; //the type of reading which the sensor shall return

    //only OiConfigState can access this method from outside this class
    //void setIsSaved(bool isSaved);
};

Q_DECLARE_METATYPE( MeasurementConfig )
Q_DECLARE_METATYPE( MeasurementConfig* )

#endif // MEASUREMENTCONFIG_H
