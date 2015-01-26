#ifndef SENSORCONTROL_H
#define SENSORCONTROL_H

#include <QMutex>
#include <QDebug>
#include <QTime>
#include "connectionconfig.h"
#include "station.h"
#include "measurementconfig.h"
#include "oiemitter.h"
#include "sensorconfiguration.h"

class SensorListener;
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

    //instrument
    Sensor *instrument;
    QList<Sensor*> usedSensors;
    SensorConfiguration InstrumentConfig;
    SensorListener *instrumentListener;

    QThread listenerThread;

   // bool isReadingStreamActive;
   // bool isSensorStatStreamActive;

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

    OiEmitter& getOiEmitter();

signals:
    void commandFinished(bool);
    void recalcFeature(Feature*);
    void activateStatStream();
    void activateReadingStream(int);
    void measurementFinished(int,bool);


public slots:

    //measure
    void measure(Geometry *geom,bool isActiveCoordSys);

    //data stream
    void readingStream(int);
    void sensorStatsStream();

    //sensor actions
    void move(double, double, double, bool);
    void move(double, double, double);
    void initialize();
    void motorState();
    void home();
    void toggleSight();
    void connectSensor(ConnectionConfig *connConfig);
    void disconnectSensor();
    void compensation();
    void doSelfDefinedAction(QString s);
    void stopReadingStream();
    void stopStatStream();

    void copyMe(SensorControl *sc);

    void addReading(Reading* r, Geometry* geom, bool isActiveCoordSys);

    Configuration::ReadingTypes getTypeOfReadingStream();

private:

    enum streamType{
        eReadingStream,
        eSenorStats,
        eNoStream
    };

    Station *myStation;
    QMutex locker;
    OiEmitter myEmitter;

    streamType t;
    int typeOfReadingStream;

    void storeReadings(QList<Reading*>readings, Geometry* geom, bool isActiveCoordSys);
    void saveReading(Reading* r, Geometry* geom, bool isActiveCoordSys);

    bool sendActivateStream();
    bool sendDeactivateStream();
    bool checkSensor();

private slots:
    void streamLostSignal();


};

#endif // SENSORCONTROL_H
