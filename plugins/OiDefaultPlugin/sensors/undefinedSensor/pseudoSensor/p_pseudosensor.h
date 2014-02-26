#ifndef P_PSEUDOSENSOR_H
#define P_PSEUDOSENSOR_H

#include "sensor.h"

class PseudoSensor : public Sensor
{
public:

    PseudoSensor();

     bool accept(SensorControl*, Configuration::SensorFunctionalities){return false;}

     QList<Configuration::ReadingTypes>* getSupportedReadingTypes();

     QList<Configuration::ConnectionTypes>* getConnectionType();

     QMap<QString,int>* getIntegerParameter();

     QMap<QString,double>* getDoubleParameter();

      QMap <QString, QStringList>* getStringParameter();

      QMap<QString, double>* getUndefinedSigma();

    //! laser tracker measures a point and returns an observation
    QList<Reading*> measure(MeasurementConfig *mc);

    void dataStream() ;

    void sendCommandString(QString);

    //! checks if the measurementconfig is vaild
    bool checkMeasurementConfig(MeasurementConfig*);

    //! connect app with laser tracker
    bool connectSensor(ConnectionConfig*);

    //! disconnect app with laser tracker
    bool disconnectSensor();

    PluginMetaData* getMetaData();

};

#endif // P_PSEUDOSENSOR_H
