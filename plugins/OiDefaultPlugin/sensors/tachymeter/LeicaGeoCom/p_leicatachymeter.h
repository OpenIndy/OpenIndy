#ifndef P_LEICATACHYMETER_H
#define P_LEICATACHYMETER_H

#include <QtGlobal>
#include <QDateTime>
#include <QObject>
#include <QDebug>
#include <QtSerialPort/QSerialPort>
#include <QRegExp>
#include <QThread>
#include <QStringList>
#include <QVariantMap>
#include "pi_totalstation.h"


class LeicaTachymeter : public TotalStation
{

    
public:
    LeicaTachymeter();

public:

    QList<Configuration::ReadingTypes>* getSupportedReadingTypes();

    QList<Configuration::ConnectionTypes>* getConnectionType();

    QMap<QString,int>* getIntegerParameter();

    QMap<QString,double>* getDoubleParameter();

    QMap <QString, QStringList>* getStringParameter();

    /*default accuracy
     *keys:
     * sigmaAzimuth sigmaAzimuth sigmaDistance
     * sigmaXyz
     * sigmaTempDeg
     * sigmaAngleXZ
     * sigmaAngleYZ
     */
    QMap<QString,double>* getDefaultAccuracy();

    //! measure
    QList<Reading*> measure(MeasurementConfig* mc);

    //! connect to sensor
    virtual bool connectSensor(ConnectionConfig* connConfig);

    //! disconnect to sensor
    virtual bool disconnectSensor();

    //!datastream
    void dataStream();

    //! executes own defined command string
    void sendCommandString(QString cmd) ;

    //! checks if the measurementconfig is vaild
    bool checkMeasurementConfig(MeasurementConfig* mc);

    PluginMetaData* getMetaData();

protected:

    bool move(double azimuth, double zenith, double distance,bool isrelativ);

    bool toggleSightOrientation();

private:

    QSerialPort *serial;


    QList<Reading*> measurePolar(MeasurementConfig*);
    QList<Reading*> measureDistance(MeasurementConfig*);
    QList<Reading*> measureDirection(MeasurementConfig*);
    QList<Reading*> measureCartesian(MeasurementConfig*);

    QSerialPort::BaudRate myBaudRate;
    QSerialPort::DataBits myDataBits;
    QSerialPort::Parity myParity;
    QSerialPort::StopBits myStopBits;
    QSerialPort::FlowControl myFlowControl;

    QString receive();
    void getError(QSerialPort::SerialPortError);
    bool executeEDM();
    bool executeCommand(QString);

};

#endif // P_LEICATACHYMETER_H
