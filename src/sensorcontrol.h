#ifndef SENSORCONTROL_H
#define SENSORCONTROL_H

#include <QObject>
#include <QPointer>
#include <QThread>
#include <QMutex>
#include <QTime>

#include "connectionconfig.h"
#include "station.h"
#include "sensor.h"
//#include "measurementconfig.h"
//#include "oiemitter.h"
//#include "sensorconfiguration.h"

class SensorListener;
//class Station;

/*!
 * \brief The SensorControl class
 * A sensor control object belongs to a station and executes all sensor actions on a seperate thread.
 */
class SensorControl : public QObject
{
    Q_OBJECT

public:

    SensorControl(QPointer<Station> &station, QObject *parent = 0);

    ~SensorControl();

public slots:

    //####################################
    //get or set sensor control attributes
    //####################################

    void setSensor(const QPointer<Sensor> &sensor);

    //####################
    //start sensor actions
    //####################

    void connectSensor(const ConnectionConfig &connConfig);
    void disconnectSensor();

    void measure(const QPointer<Geometry> &geom, const bool &isActiveCoordSys);

    void readingStream(const bool &start, const ReadingTypes &readingType);
    void sensorStatsStream(const bool &start);

    void move(const double &azimuth, const double &zenith, const double &distance, const bool &isRelative);
    void move(const double &x, const double &y, const double &z);
    void initialize();
    void motorState();
    void home();
    void toggleSight();
    void compensation();
    void selfDefinedAction(const QString &action);

    //void copyMe(SensorControl *sc);
    //void addReading(Reading* r, Geometry* geom, bool isActiveCoordSys);
    //ReadingTypes getTypeOfReadingStream();

signals:

    //##############################################
    //signals emitted after sensor actions were done
    //##############################################

    void commandFinished(const bool &success);
    void measurementFinished(const int &geomId, const bool &success);

    void recalcFeature(const QPointer<Feature> &feature);

    //void activateStatStream();
    //void activateReadingStream(int);

private:

    //#################################
    //general sensor control attributes
    //#################################

    QPointer<Station> station;

    QPointer<Sensor> sensor;
    QList<QPointer<Sensor> > usedSensors;

    //SensorConfiguration InstrumentConfig;

    QPointer<SensorListener> sensorListener;
    QThread listenerThread;

    QMutex locker;
    //OiEmitter myEmitter;

   // bool isReadingStreamActive;
   // bool isSensorStatStreamActive;

    //################################################################
    //helper attributes for the sensor plugins to get their parameters
    //################################################################

    double moveAzimuth;
    double moveZenith;
    double moveDistance;
    bool moveIsRelative;
    double moveX;
    double moveY;
    double moveZ;
    QString action;
    //bool isMState;

    //OiEmitter& getOiEmitter();

private slots:

    /*enum streamType{
        eReadingStream,
        eSenorStats,
        eNoStream
    };*/

    void saveMeasurementResults(const QList<QPointer<Reading> > &readings, const QPointer<Geometry> &geom, const bool &isActiveCoordSys);

    void activateSensorStream();
    void deactivateSensorStream();

    //streamType t;
    //int typeOfReadingStream;

    //void storeReadings(QList<Reading*>readings, Geometry* geom, bool isActiveCoordSys);
    //void saveReading(Reading* r, Geometry* geom, bool isActiveCoordSys);

    //bool sendActivateStream();
    //bool sendDeactivateStream();
    //bool checkSensor();
    //void streamLostSignal();


};

#endif // SENSORCONTROL_H
