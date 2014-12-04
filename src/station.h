#ifndef STATION_H
#define STATION_H

#include <QObject>
#include <QList>
#include <QThread>
#include "feature.h"
#include "configuration.h"
#include "observation.h"
#include "sensor.h"
#include "sensorcontrol.h"
#include "measurementconfig.h"
#include "reading.h"
#include "coordinatesystem.h"
#include "sensorconfiguration.h"
#include "point.h"
#include "oisensoremitter.h"

class SensorControl;
class Reading;
class CoordinateSystem;
class Sensor;

/*!
 * \brief The Station class
 * The station contains a list of all readings made from this station, the coordinate system that belongs
 * to this station, sensorcontroller. The station slots emit a station signal, which starts
 * a sensor action  of the sensorcontroller(slot). The sensorcontroller runs on a seperate
 * thread (stationThread). If the sensor action is completed, the actionFinished signal will be emitted.
 */
class Station : public Feature
{
    Q_OBJECT
public:

    Station(QString name);
    Station();
    virtual ~Station();

    bool getIsActiveStation();
    bool setActiveStationState(bool isActiveStation);

    Point *position;

    SensorControl *sensorPad;

    //coordinatesystem
    CoordinateSystem *coordSys;

    //readings
    QList<Reading*> readingsXyz;
    QList<Reading*> readingsDir;
    QList<Reading*> readingsDist;
    QList<Reading*> readingsPol;
    QList<Reading*> readingsLevel;
    QList<Reading*> readingsTemperatur;
    QList<Reading*> readingsUndefined;


signals:
    //evertime the sensor finised an action, this signal will be emitted
    void actionFinished(bool);

    //starts the measurement
    void startMeasure(Geometry *geom, bool isActiveCoordSys) const;

    //data streams
    void startReadingStream(int) const;
    void startSensorStatsStream() const;
    void stopReadingStream() const;
    void stopSensorStatsStream() const;

    //sensor actions (station signals)
    void startMove(double, double, double, bool) const;
    void startMove(double, double, double) const;
    void startInitialize() const;
    void startMotorState() const;
    void startHome() const;
    void startToggleSight() const;
    void startConnect(ConnectionConfig*) const;
    void startDisconnect() const;
    void startCompensation() const;
    void startSelfDefinedAction(QString s) const;

    void sendToConsole(QString);

    void activeStationChanged(int stationId);

public slots:

    //thread control
    void stopThread();
    void startThread();

    void setInstrumentConfig(SensorConfiguration sensorConfig);
    SensorConfiguration getInstrumentConfig();

    //measurement
    void emitStartMeasure(Geometry *geom, bool isActiveCoordSys) const;

    //data stream
    void emitStartReadingStream(int readingType) const;
    void emitStopReadingStream() const;
    void emitStartSensorStatsStream() const;
    void emitStopSensorStatsStream() const;

    //sensor actions (slots to emit the station signals)
    void emitStartMove(double, double, double, bool) const;
    void emitStartMove(double, double, double) const;
    void emitStartInitialize() const;
    void emitStartMotorState() const;
    void emitStartHome() const;
    void emitStartToggleSight() const;
    void emitStartConnect(ConnectionConfig*) const;
    void emitStartDisconnect() const;
    void emitStartCompensation() const;
    void emitSelfDefinedAction(QString s) const;

    OiSensorEmitter* getActiveSensorEmitter();

    QString getDisplayX() const;
    QString getDisplayY() const;
    QString getDisplayZ() const;
    QString getDisplayIsCommon() const;
    QString getDisplayIsNominal() const;
    QString getDisplaySolved() const;
    QString getDisplayMConfig() const;
    QString getDisplayStdDev() const;

private slots:
    //! will be emitted when sensor action is completed
    void emitActionFinished(bool wasSuccessful);
    void writeToConsole(QString);

private:
    // thread for the SensorControl
    QThread stationThread;

    bool isActiveStation;

public:
    void recalc();

    QDomElement toOpenIndyXML(QDomDocument &xmlDoc);
    bool fromOpenIndyXML(QDomElement &xmlElem);

};

#endif // STATION_H
