#ifndef STATION_H
#define STATION_H

#include <QObject>
#include <QList>
#include <QPointer>

#include "feature.h"
#include "point.h"

class SensorControl;
class Reading;
class CoordinateSystem;
class Sensor;

/*!
 * \brief The Station class
 */
class Station : public Feature
{
    Q_OBJECT

public:
    explicit Station(QObject *parent = 0);
    explicit Station(const QString &name, QObject *parent = 0);

    Station(const Station &copy, QObject *parent = 0);

    Station &operator=(const Station &copy);

    ~Station();

    //#############################
    //get or set station attributes
    //#############################

    const bool &getIsActiveStation() const;
    void setActiveStationState(const bool &isActiveStation);

    const QPointer<Point> &getPosition() const;

    const QPointer<CoordinateSystem> &getCoordinateSystem() const;

    void setSensor(const QPointer<Sensor> &sensor);

    //void setInstrumentConfig(SensorConfiguration sensorConfig);
    //SensorConfiguration getInstrumentConfig();

    //###########################
    //reexecute the function list
    //###########################

    void recalc();

    //#################
    //save and load XML
    //#################

    QDomElement toOpenIndyXML(QDomDocument &xmlDoc);
    bool fromOpenIndyXML(QDomElement &xmlElem);


    //ReadingTypes getReadingStreamType();

public slots:

    //#####################
    //control sensor thread
    //#####################

    void stopThread();
    void startThread();



    //void connect(const ConnectionConfig &config) const;
    //void disconnect() const;

    //void measure(const QPointer<Geometry> &geom, const bool &isActiveCoordSys) const;

    //void readingStream(const bool &start, const ReadingTypes &readingType) const;
    //void sensorStateStream(const bool &start) const;

    //void move(const double &azimuth, const double &zenith, const double &distance, const bool &isRelative) const;
    //void move(const double &x, const double &y, const double &z) const;
    //void initialize() const;
    //void motorState() const;
    //void home() const;
    //void toggleSight() const;
    //void compensation() const;
    //void selfDefinedAction(const QString &action) const;


    //OiSensorEmitter* getActiveSensorEmitter();

    //QString getDisplayX() const;
    //QString getDisplayY() const;
    //QString getDisplayZ() const;
    //QString getDisplayIsCommon() const;
    //QString getDisplayIsNominal() const;
    //QString getDisplaySolved() const;
    //QString getDisplayMConfig() const;
    //QString getDisplayStdDev() const;

signals:

    //#######################################
    //signals to inform about station changes
    //#######################################

    void activeStationChanged(const int &stationId);
    void sensorChanged(const int &featureId);

    //###############################
    //signals to start sensor actions
    //###############################

    void connectSensor();
    void disconnectSensor();

    void measure(const QPointer<Geometry> &geom, const bool &isActiveCoordSys);

    void readingStream(const bool &start, const ReadingTypes &readingType);
    void sensorStateStream(const bool &start);

    void move(const double &azimuth, const double &zenith, const double &distance, const bool &isRelative);
    void move(const double &x, const double &y, const double &z);
    void initialize();
    void motorState();
    void home();
    void toggleSight();
    void compensation();
    void selfDefinedAction(const QString &action);

    //#################################################
    //signals emitted when a sensor action was finished
    //#################################################

    void actionFinished(bool);

    //void sendToConsole(QString);

private:

    //##########################
    //general station attributes
    //##########################

    bool isActiveStation;

    QPointer<Point> position; //the position of the station

    QPointer<SensorControl> sensorControl; //sensor communication
    QThread stationThread; //thread the sensor control runs on

    QPointer<CoordinateSystem> stationSystem; //the corresponding coordinate system

    //#############################
    //readings made by this station
    //#############################

    QList<QPointer<Reading> > cartesianReadings;
    QList<QPointer<Reading> > directionReadings;
    QList<QPointer<Reading> > distanceReadings;
    QList<QPointer<Reading> > polarReadings;
    QList<QPointer<Reading> > levelReadings;
    QList<QPointer<Reading> > temperatureRadings;
    QList<QPointer<Reading> > undefinedReadings;

private slots:

    //##############
    //helper methods
    //##############

    void stationNameChanged(const int &featureId, const QString &oldName);
    void connectSensorControl();

    //! will be emitted when sensor action is completed
    //void emitActionFinished(bool wasSuccessful);
    //void writeToConsole(QString);

};

#endif // STATION_H
