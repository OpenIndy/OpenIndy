#ifndef SENSORLISTENER_H
#define SENSORLISTENER_H

#include <QObject>
#include <QThread>
#include <QTime>
#include <QMap>
#include <QVariantMap>

#include "sensor.h"

class SensorControl;

/*!
 * \brief The SensorListener class
 * The sensor listener handles all sensor streams and is controlled by SensorControl
 */
class SensorListener : public QObject
{
    friend class SensorControl;
    Q_OBJECT

public:
    SensorListener(QMutex &locker, QObject *parent = 0);

    ~SensorListener();

signals:

    //##############################
    //inform about streaming results
    //##############################

    void realTimeReading(const QVariantMap &reading) const;
    void realTimeStatus(const QMap<QString, QString> &status) const;

    void connectionLost() const;
    void connectionReceived() const;

    void isReadyForMeasurement(const bool &isReady) const;

    //#######################
    //special sensor messages
    //#######################

    void sensorMessage(const Sensor::SensorMessage &type, const QString &msg);

private:

    //#################
    //get or set sensor
    //#################

    void setSensor(const QPointer<Sensor> &sensor);
    const QPointer<Sensor> &getSensor() const;

private slots:

    //#######################
    //start or stop streaming
    //#######################

    void startStreaming();

    //################################
    //get or set reading stream format
    //################################

    ReadingTypes getReadingStreamFormat() const;
    void setReadingStreamFormat(ReadingTypes streamFormat);

private:

    //##########################
    //sensor listener attributes
    //##########################

    QPointer<Sensor> sensor;

    ReadingTypes streamFormat;

    QMutex &locker;

};

#endif // SENSORLISTENER_H
