#ifndef SENSORLISTENER_H
#define SENSORLISTENER_H

#include <QObject>
#include <QThread>
#include <QTime>
#include <QMap>
#include <QVariantMap>

#include "sensor.h"

/*!
 * \brief The SensorListener class
 * The sensor listener handles all sensor streams and is controlled by SensorControl
 */
class SensorListener : public QObject
{
    Q_OBJECT

public:
    SensorListener(const QPointer<Sensor> &sensor, QObject *parent = 0);

    ~SensorListener();

    //#################
    //get or set sensor
    //#################

    void setInstrument(QPointer<Sensor> sensor);
    QPointer<Sensor> getInstrument() const;

    //#####################
    //check stream activity
    //#####################

    bool getIsStreamActive() const;

public slots:

    //#######################
    //start or stop streaming
    //#######################

    void startStatusStream();
    void startReadingStream(const ReadingTypes &streamFormat);

    void abortSensorAction();
    
signals:

    //##############################
    //inform about streaming results
    //##############################

    void sendReadingMap(const QVariantMap &stream);
    void sendSensorStats(const QMap<QString, QString> &stream);
    void connectionLost();

private:

    //##########################
    //sensor listener attributes
    //##########################

    QPointer<Sensor> sensor;

    bool isStreamActive;
    //bool isStreamFinished;

};

#endif // SENSORLISTENER_H
