#ifndef P_PSEUDOSENSOR_H
#define P_PSEUDOSENSOR_H

#include "sensor.h"

class PseudoSensor : public Sensor
{
public:

    PseudoSensor();

    QList<Configuration::ReadingTypes>* getSupportedReadingTypes() const;
    QList<Configuration::SensorFunctionalities> getSupportedSensorActions() const;
    QList<Configuration::ConnectionTypes>* getConnectionType() const;

    PluginMetaData* getMetaData() const;

    QMap<QString,int>* getIntegerParameter() const;
    QMap<QString,double>* getDoubleParameter() const;
    QMap <QString, QStringList>* getStringParameter() const;
    QStringList selfDefinedActions() const;
    bool doSelfDefinedAction(QString a);

    QString getUndefinedReadingName() const;
    QMap<QString,double>* getDefaultAccuracy() const;

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
