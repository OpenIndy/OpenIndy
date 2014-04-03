#ifndef STATION_H
#define STATION_H

#include <QObject>
#include <QList>
#include <QThread>
#include "feature.h"
#include "configuration.h"
#include "console.h"
#include "observation.h"
#include "sensor.h"
#include "sensorcontrol.h"
#include "measurementconfig.h"
#include "reading.h"
#include "coordinatesystem.h"
#include "sensorconfiguration.h"
#include "point.h"

class SensorControl;
class Reading;
class CoordinateSystem;
class Sensor;


/*!
 * \brief The Station class
 * The station contains a list of all readings made from this station, the coordinate system that belongs
 * to this station, a sensor and a sensorcontroller. The station slots emit a station signal, which starts
 * a sensor action  of the sensorcontroller(slot). The sensorcontroller runs on a seperate
 * thread (stationThread). If the sensor action is completed, the actionFinished signal will be emitted.
 */
class Station : public QObject, public Feature
{
    Q_OBJECT
public:

    //constructor
    Station(QString name);
    virtual ~Station();

    Point *position;

    //instrument
    Sensor *instrument;
    QList<Sensor*> usedSensors;

    SensorControl *sensorPad;

    //coordinatesystem
    CoordinateSystem *coordSys;

    //readings
    QList<Reading*> readingsXyz;
    QList<Reading*> readingsDir;
    QList<Reading*> readingsDist;
    QList<Reading*> readingsPol;


signals:
    //evertime the sensor finised an action, this signal will be emitted
    void actionFinished(bool);

    //starts the measurement
    void startMeasure(Geometry *geom, bool isActiveCoordSys);

    //data stream
    void startStream();

    //sensor actions (station signals)
    void startMove(double, double, double, bool);
    void startMove(double, double, double);
    void startInitialize();
    void startMotorState(bool);
    void startHome();
    void startToggleSight();
    void startConnect(ConnectionConfig*);
    void startDisconnect();
    void startCompensation();
    void startSendCommand(QString);


public slots:

    //thread control
    void stopThread();
    void startThread();

    void setInstrumentConfig(SensorConfiguration *sensorConfig);
    SensorConfiguration* getInstrumentConfig();

    //measurement
    void emitStartMeasure(Geometry *geom, bool isActiveCoordSys);

    //data stream
    void emitStartStream();
    void stopStream();

    //sensor actions (slots to emit the station signals)
    void emitStartMove(double, double, double, bool);
    void emitStartMove(double, double, double);
    void emitStartInitialize();
    void emitStartMotorState(bool);
    void emitStartHome();
    void emitStartToggleSight();
    void emitStartConnect(ConnectionConfig*);
    void emitStartDisconnect();
    void emitStartCompensation();
    void emitStartCommand(QString);

    QString getDisplayX() const;
    QString getDisplayY() const;
    QString getDisplayZ() const;
    QString getDisplayIsCommon() const;
    QString getDisplayIsNominal() const;
    QString getDisplayObs() const;
    QString getDisplaySolved() const;
    QString getDisplayMConfig() const;
    QString getDisplayStdDev() const;

private slots:
    //! will be emitted when sensor action is completed
    void emitActionFinished(bool wasSuccessful);

private:
    // thread for the SensorControl
    QThread stationThread;
    SensorConfiguration *InstrumentConfig;

 public:
    void recalc();

    virtual bool toOpenIndyXML(QXmlStreamWriter& stream);
    virtual ElementDependencies fromOpenIndyXML(QXmlStreamReader& xml);

};

#endif // STATION_H
