#ifndef SENSOR_H
#define SENSOR_H

#include <QString>
#include <QMap>
#include <QVariant>
#include "sensorcontrol.h"
#include "oiemitter.h"
#include "pluginmetadata.h"
#include "sensorconfiguration.h"

class Reading;
class MeasurementConfig;
class SensorControl;

/*!
 * \brief The Sensor class
 * Father class for all sensor types. The Sensor has four important methodes.
 * The measure methode execute a measurement defined by the MeasurementConfig
 * Object. The accept methode execute an sensor action defined by the Configuration
 * Enumeration.
 *
 */
class Sensor
{
public:
//constructor
    virtual ~Sensor(){}

//attributes
    bool isConnected;
    bool dataStreamIsActive;
    Configuration::ReadingTypes streamFormat;
    OiEmitter myEmitter;
    SensorConfiguration* myConfiguration;

//methods

    //get sensor capabilities
    virtual PluginMetaData* getMetaData() = 0;

    virtual QList<Configuration::ReadingTypes>* getSupportedReadingTypes() = 0;
    virtual QList<Configuration::ConnectionTypes>* getConnectionType() = 0;

    virtual QMap<QString,int>* getIntegerParameter() = 0;
    virtual QMap<QString,double>* getDoubleParameter() = 0;
    virtual QMap <QString, QStringList>* getStringParameter() = 0;

    virtual QString getUndefinedReadingName(){return "undefined";}
    virtual QMap<QString,double>* getUndefinedSigma(){return NULL;}
    void setSensorConfiguration(SensorConfiguration* sConfig){myConfiguration = sConfig;}

    //sensor control methods
    virtual bool accept(SensorControl*, Configuration::SensorFunctionalities) = 0;

    virtual bool connectSensor(ConnectionConfig*) = 0;
    virtual bool disconnectSensor() = 0;

    virtual QList<Reading*> measure(MeasurementConfig*) = 0;

    virtual void dataStream() = 0;
    virtual void sendCommandString(QString) = 0;

    virtual bool checkMeasurementConfig(MeasurementConfig*)=0;

};

#define Sensor_iidd "de.openIndy.Plugin.Sensor.v001"

#endif // SENSOR_H
