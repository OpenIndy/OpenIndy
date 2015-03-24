#ifndef SENSORCONFIGURATION_H
#define SENSORCONFIGURATION_H

#include <QtXml>

#include "connectionconfig.h"
#include "oivec.h"

class Sensor;

class OiConfigState;

struct Accuracy{

    Accuracy(){
        sigmaXyz = OiVec(3);
        sigmaAzimuth = 0.0;
        sigmaZenith = 0.0;
        sigmaDistance = 0.0;
        sigmaTemp = 0.0;
        sigmaAngleXZ = 0.0;
        sigmaAngleYZ = 0.0;
    }

    double sigmaAzimuth;
    double sigmaZenith;
    double sigmaDistance;
    OiVec sigmaXyz;
    double sigmaTemp;
    QMap<QString,double> sigmaUndefined;
    double sigmaAngleXZ;
    double sigmaAngleYZ;
};

class SensorConfiguration
{
    friend class OiConfigState;

public:
    SensorConfiguration();

    //! compare the attributes of both sensor configs
    friend bool operator==(const SensorConfiguration &left, const SensorConfiguration &right){

        //TODO compare sensor config attributes

        /*if(){
            return true;
        }else{
            return false;
        }*/

        return false;

    }

    QString getName() const;
    QString getDisplayName() const;
    bool getIsSaved() const;
    bool getIsValid() const;

    bool setName(QString name);

    ConnectionConfig *connConfig;
    SensorTypes instrumentType;

    QMap<QString, int> integerParameter;
    QMap<QString, double> doubleParameter;
    QMap<QString, QString> stringParameter;

    Accuracy sigma;

    Sensor *mySensor; //pointer to sensor to be able to access information about the sensor plugin

    QString pluginName;
    QString sensorName;

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

#endif // SENSORCONFIGURATION_H
