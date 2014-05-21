#ifndef P_PSEUDOSENSOR_H
#define P_PSEUDOSENSOR_H

#include "sensor.h"

class PseudoSensor : public Sensor
{
public:

    PseudoSensor();

    QList<Configuration::ReadingTypes>* getSupportedReadingTypes();
    QList<Configuration::SensorFunctionalities> getSupportedSensorActions();
    QList<Configuration::ConnectionTypes>* getConnectionType();

    PluginMetaData* getMetaData();

    QMap<QString,int>* getIntegerParameter();
    QMap<QString,double>* getDoubleParameter();
    QMap <QString, QStringList>* getStringParameter();
    QStringList selfDefinedActions();
    bool doSelfDefinedAction(QString a);

    QString getUndefinedReadingName();
    QMap<QString,double>* getDefaultAccuracy();

    bool accept(SensorControl*, Configuration::SensorFunctionalities){return false;}

    void abortAction();

    //! connect app with laser tracker
    bool connectSensor(ConnectionConfig*);

    //! disconnect app with laser tracker
    bool disconnectSensor();

    //! laser tracker measures a point and returns an observation
    QList<Reading*> measure(MeasurementConfig *mc);

    //! stream
    QVariantMap readingStream(Configuration::ReadingTypes streamFormat);

    //! getConnectionState
    bool getConnectionState();

    //! return ready state of the sensor
    bool isReadyForMeasurement();

    //!sensor stats
    QMap<QString,QString> getSensorStats();

    //!checks if sensor is busy
    bool isBusy();

private:
    bool isConnected;


};

#endif // P_PSEUDOSENSOR_H
