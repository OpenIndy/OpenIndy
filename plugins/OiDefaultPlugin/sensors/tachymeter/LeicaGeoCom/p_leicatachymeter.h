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
#include "totalstation.h"

/*!
 * \brief The LeicaTachymeter class
 */
class LeicaTachymeter : public TotalStation
{
    Q_OBJECT

protected:

    //############################
    //sensor initialization method
    //############################

    void init();

public:

    //########################
    //sensor state and actions
    //########################

    bool doSelfDefinedAction(const QString &action);

    //! abort a running action
    bool abortAction();

    //! connect to sensor
    bool connectSensor();

    //! disconnect to sensor
    bool disconnectSensor();

    //! measure
    QList<QPointer<Reading> > measure(const MeasurementConfig &mConfig);

    //! stream
    QVariantMap readingStream(const ReadingTypes &streamFormat);

    //! getConnectionState
    bool getConnectionState();

    //! return ready state of the sensor
    bool getIsReadyForMeasurement();

    //!sensor stats
    QMap<QString, QString> getSensorStatus();

    //!checks if sensor is busy
    bool getIsBusy();

protected:

    //! move totalstation to specified position
    bool move(const double &azimuth, const double &zenith, const double &distance, const bool &isRelative);

    //! move total station to specified xyz position
    bool move(const double &x, const double &y, const double &z);

    //! toggle between frontside and backside
    bool toggleSightOrientation();

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

    QList<QPointer<Reading> > measurePolar(const MeasurementConfig &mConfig);
    QList<QPointer<Reading> > measureDistance(const MeasurementConfig &mConfig);
    QList<QPointer<Reading> > measureDirection(const MeasurementConfig &mConfig);
    QList<QPointer<Reading> > measureCartesian(const MeasurementConfig &mConfig);

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

    SensorFaces getCurrentFace(double zenith);

    bool setTargetTypeMeasure();
    bool setTargetTypeStream();
    bool setAdjustMode();

    void stopTrackingAfterMeasure();

    bool laserOn;
    bool fineAdjusted;

    bool measureWatchWindow;
    bool watchWindowOpen;
    ReadingTypes currentStreamFormat;
    void stopWatchWindowForMeasurement();
    void restartWatchWindowAfterMeasurement();


    QPointer<Reading> getQuickMeasReading(QString receive);

    void activateLaserPointer();
    void deactivateLaserPointer();

    void stopTrackingMode();

    QPointer<Reading> getStreamValues();

};

#endif // P_LEICATACHYMETER_H
