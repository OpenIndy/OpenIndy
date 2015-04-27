#ifndef P_PSEUDOTRACKER_H
#define P_PSEUDOTRACKER_H

#include <QtGlobal>
#include <QDateTime>
#include <QObject>
#include <QDebug>
#include <QThread>
#include <QFile>
#include <QMap>
#include <QString>
#include <cmath>

#include "lasertracker.h"
#include "oimat.h"

/*!
 * \brief The PseudoTracker class
 */
class PseudoTracker : public LaserTracker
{
    Q_OBJECT

protected:

    //############################
    //sensor initialization method
    //############################

    void init();

public:

    //########################
    //sensor state and actions
    //########################

    bool doSelfDefinedAction(const QString &action);
    bool abortAction();

    //! connect app with laser tracker
    bool connectSensor();

    //! disconnect app with laser tracker
    bool disconnectSensor();

    //! laser tracker measures a point and returns an observation
    QList<QPointer<Reading> > measure(const MeasurementConfig &mConfig);

    //! stream
    QVariantMap readingStream(const ReadingTypes &streamFormat);

    //! getConnectionState
    bool getConnectionState();

    //! return ready state of the sensor
    bool getIsReadyForMeasurement();

    //!sensor stats
    QMap<QString, QString> getSensorStatus();

    //!checks if sensor is busy
    bool getIsBusy();

protected:

    //! starts initialization
    bool initialize();

    //! move laser tracker to specified position
    bool move(const double &azimuth, const double &zenith, const double &distance, const bool &isRelative);

    bool move(const double &x, const double &y, const double &z);

    //! sets laser tracke to home position
    bool home();

    //! turns motors on or off
    bool changeMotorState();

    //! toggle between frontside and backside
    bool toggleSightOrientation();

    //! compensation
    bool compensation() ;

private:
    QList<QPointer<Reading> > measurePolar(const MeasurementConfig &mConfig);
    QList<QPointer<Reading> > measureDistance(const MeasurementConfig &mConfig);
    QList<QPointer<Reading> > measureDirection(const MeasurementConfig &mConfig);
    QList<QPointer<Reading> > measureCartesian(const MeasurementConfig &mConfig);

    //methodes to generate random value
    double randomX(int d, double m, double s);
    double randomNorm();
    double randomTriangular(double c, double a,double b);

    void noisyPolarReading(ReadingPolar &r);

    //################
    //helper variables
    //################

    double myAzimuth;
    double myZenith;
    double myDistance;

    bool myMotor;
    bool myInit;
    bool myCompIt;
    int side;
    bool isConnected;

};

#endif // P_PSEUDOTRACKER_H
