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

    QList<Configuration::ReadingTypes>* getSupportedReadingTypes() const;
    QList<Configuration::SensorFunctionalities> getSupportedSensorActions() const;
    QList<Configuration::ConnectionTypes>* getConnectionType() const;

    PluginMetaData* getMetaData() const;

    QMap<QString,int>* getIntegerParameter() const;
    QMap<QString,double>* getDoubleParameter() const;
    QMap <QString, QStringList>* getStringParameter() const;
    QStringList selfDefinedActions() const;
    bool doSelfDefinedAction(QString a);

    /*default accuracy
     *keys:
     * sigmaAzimuth sigmaAzimuth sigmaDistance
     * sigmaXyz
     * sigmaTempDeg
     * sigmaAngleXZ
     * sigmaAngleYZ
     */
    QMap<QString,double>* getDefaultAccuracy() const;

    //! abort a running action
    void abortAction();

    //! connect to sensor
    virtual bool connectSensor(ConnectionConfig* connConfig);

    //! disconnect to sensor
    virtual bool disconnectSensor();

    //! measure
    QList<Reading*> measure(MeasurementConfig* mc);

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

protected:

    //! move totalstation to specified position
    bool move(double azimuth, double zenith, double distance,bool isrelativ);

    //! move total station to specified xyz position
    bool move(double x, double y, double z);

    //! toggle between frontside and backside
    bool toggleSightOrientation();

    //! check ATR state
    bool getATRState();

    //! set ATR state
    bool setATRState(QString ATRstate);

    //! check LOCK state
    bool getLOCKState();

    //! deactivate lock state
    void deactiveLockState();

    //! sets the LOCK state
    bool setLOCKState(QString currentState);

    //! starts target tracking
    bool startTargetTracking();

    //! starts the fine adjustment for ATR on prism
    bool fineAdjust();


private:

    QSerialPort *serial;

    //int getEDMMode();

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

    bool checkCommandRC(QString command);

    Configuration::Faces getCurrentFace(double zenith);

    bool setTargetTypeMeasure();
    bool setTargetTypeStream();

};

#endif // P_LEICATACHYMETER_H
