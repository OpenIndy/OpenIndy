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

    //-----get sensor capabilities-----

    virtual QList<Configuration::ReadingTypes>* getSupportedReadingTypes() = 0;
    virtual QList<Configuration::ConnectionTypes>* getConnectionType() = 0;

    //! get meta data
    virtual PluginMetaData* getMetaData() = 0;

    //individually defined sensor parameter
    virtual QMap<QString,int>* getIntegerParameter() = 0;
    virtual QMap<QString,double>* getDoubleParameter() = 0;
    virtual QMap <QString, QStringList>* getStringParameter() = 0;

    //individually defined reading type
    virtual QString getUndefinedReadingName(){return "undefined";}
    virtual QMap<QString,double>* getDefaultAccuracy() = 0;

    //! sensor configuration
    void setSensorConfiguration(SensorConfiguration* sConfig){myConfiguration = sConfig;}

    //-----sensor actions-----

    /*! checks given sensor functionality and nd calls the appropriate internal method
     * of the instance*/
    virtual bool accept(SensorControl*, Configuration::SensorFunctionalities) = 0;

    //! connect app with sensor
    virtual bool connectSensor(ConnectionConfig*) = 0;

    //! disconnect app with sensor
    virtual bool disconnectSensor() = 0;

    //! sensor measures a entity and returns a list of readings
    virtual QList<Reading*> measure(MeasurementConfig*) = 0;

    //stream
    virtual void dataStream() = 0;

    //exec individually defined command strings
    virtual void sendCommandString(QString) = 0;

    //! checks if the measurementconfig is vaild
    virtual bool checkMeasurementConfig(MeasurementConfig*)=0;

};

#define Sensor_iidd "de.openIndy.Plugin.Sensor.v001"

#endif // SENSOR_H
