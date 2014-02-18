#ifndef SENSORCONTROL_H
#define SENSORCONTROL_H

#include <QMutex>
#include <QDebug>
#include "connectionconfig.h"
#include "station.h"
#include "measurementconfig.h"



class Station;




/*!
 * \brief The SensorControl class
 * the sensor control object belongs to a station and executes all sensor actions on a seperate thread.
 */
class SensorControl : public QObject
{
    Q_OBJECT

public:

    //constructor
    SensorControl(Station *st);
    ~SensorControl();

    //attributes
    double az;
    double ze;
    double dist;
    double x_;
    double y_;
    double z_;
    bool isMoveRelativ;
    bool isMState;
    QString cmd;

signals:
    void commandFinished(bool);
    void recalcFeature(Feature*);

public slots:

    //measure
    void measure(Geometry *geom,bool isActiveCoordSys);

    //data stream
    void stream();

    //sensor actions
    void move(double, double, double, bool);
    void move(double, double, double);
    void initialize();
    void motorState(bool b);
    void home();
    void sendCommandString(QString);
    void toggleSight();
    void connectSensor(ConnectionConfig *connConfig);
    void disconnectSensor();
    void compensation();

private:
    Station *myStation;
    QMutex locker;

    void storeReadings(QList<Reading*>readings, Geometry* geom, bool isActiveCoordSys);
    void saveReading(Reading* r, Geometry* geom, bool isActiveCoordSys);

};

#endif // SENSORCONTROL_H
