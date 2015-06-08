#ifndef STATION_H
#define STATION_H

#include <QObject>
#include <QList>
#include <QPointer>

#include "sensorcontrol.h"
#include "feature.h"
#include "point.h"

class Reading;
class CoordinateSystem;
class Sensor;
class ProjectExchanger;

/*!
 * \brief The Station class
 */
class Station : public Feature
{
    friend class OiJob;
    friend class ProjectExchanger;
    friend class Observation;
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

    const QPointer<SensorListener const> getSensorListener() const;

    //####################################################
    //get information about the currently connected sensor
    //####################################################

    SensorTypes getActiveSensorType() const;

    QList<ReadingTypes> getSupportedReadingTypes() const;
    QList<ConnectionTypes> getSupportedConnectionTypes() const;

    QList<SensorFunctions> getSupportedSensorActions() const;
    QStringList getSelfDefinedActions() const;

    //###########################
    //reexecute the function list
    //###########################

    void recalc();

    //#################
    //save and load XML
    //#################

    QDomElement toOpenIndyXML(QDomDocument &xmlDoc);
    bool fromOpenIndyXML(QDomElement &xmlElem);

    //###############
    //display methods
    //###############

    QString getDisplayX(const UnitType &type, const int &digits, const bool &showDiff = false) const;
    QString getDisplayY(const UnitType &type, const int &digits, const bool &showDiff = false) const;
    QString getDisplayZ(const UnitType &type, const int &digits, const bool &showDiff = false) const;

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

    void measure(const int &geomId, const MeasurementConfig &mConfig);

    void setStreamFormat(ReadingTypes streamFormat);

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

    void commandFinished(const bool &success, const QString &msg);
    void measurementFinished(const int &geomId, const QList<QPointer<Reading> > &readings);

protected:

    //######################################
    //update feature id when the job was set
    //######################################

    void setUpFeatureId();

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

    void addReadings(const int &geomId, const QList<QPointer<Reading> > &readings);
    void removeReading(const QPointer<Reading> &reading);

protected:

    //#############################################
    //pointer to OiJob that this feature belongs to
    //#############################################

    void setJob(const QPointer<OiJob> &job);

};

#endif // STATION_H
