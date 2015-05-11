#ifndef SENSORCONTROL_H
#define SENSORCONTROL_H

#include <QObject>
#include <QPointer>
#include <QThread>
#include <QMutex>
#include <QTime>

#include "sensor.h"
#include "measurementconfig.h"

class Station;
class SensorListener;
class ProjectExchanger;

/*!
 * \brief The SensorControl class
 * A sensor control object belongs to a station and executes all sensor actions on a seperate thread.
 */
class SensorControl : public QObject
{
    friend class ProjectExchanger;
    Q_OBJECT

public:
    SensorControl(QPointer<Station> &station, QObject *parent = 0);

    ~SensorControl();

public slots:

    //####################################
    //get or set sensor control attributes
    //####################################

    const QPointer<Sensor> &getSensor() const;
    void setSensor(const QPointer<Sensor> &sensor);

    const QList<QPointer<Sensor> > &getUsedSensors() const;

    const QPointer<SensorListener const> getSensorListener() const;

    void setStreamFormat(ReadingTypes streamFormat);

    //####################
    //start sensor actions
    //####################

    void connectSensor();
    void disconnectSensor();

    void measure(const int &geomId, const MeasurementConfig &mConfig);

    void move(const double &azimuth, const double &zenith, const double &distance, const bool &isRelative);
    void move(const double &x, const double &y, const double &z);
    void initialize();
    void motorState();
    void home();
    void toggleSight();
    void compensation();
    void selfDefinedAction(const QString &action);

signals:

    //##############################################
    //signals emitted after sensor actions were done
    //##############################################

    void commandFinished(const bool &success, const QString &msg);
    void measurementFinished(const int &geomId, const QList<QPointer<Reading> > &readings);

private:

    //#################################
    //general sensor control attributes
    //#################################

    QPointer<Station> station;

    QPointer<Sensor> sensor;
    QList<QPointer<Sensor> > usedSensors;

    QPointer<SensorListener> sensorListener;
    QThread listenerThread;

    QMutex locker;

};

#endif // SENSORCONTROL_H
