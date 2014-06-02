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
#include "pi_lasertracker.h"
#include "oimat.h"


class PseudoTracker : public LaserTracker
{
public:

    PseudoTracker();

    QList<Configuration::ReadingTypes>* getSupportedReadingTypes();
    QList<Configuration::SensorFunctionalities> getSupportedSensorActions();
    QList<Configuration::ConnectionTypes>* getConnectionType();

    PluginMetaData* getMetaData();

    QMap<QString,int>* getIntegerParameter();
    QMap<QString,double>* getDoubleParameter();
    QMap <QString, QStringList>* getStringParameter();
    QStringList selfDefinedActions();
    bool doSelfDefinedAction(QString a);

    /*default accuracy
     *keys:
     * sigmaAzimuth sigmaAzimuth sigmaDistance
     * sigmaX
     * sigmaY
     * sigmaZ
     * sigmaTempDeg
     * sigmaAngleXZ
     * sigmaAngleYZ
     */
    QMap<QString,double>* getDefaultAccuracy();

    //! abort a running action
    void abortAction();

    //! connect app with laser tracker
    bool connectSensor(ConnectionConfig*);

    //! disconnect app with laser tracker
    bool disconnectSensor();

    //! laser tracker measures a point and returns an observation
    QList<Reading*> measure(MeasurementConfig *mc);

    //! stream
    QVariantMap readingStream(Configuration::ReadingTypes streamFormat);

    //! getConnectionState
    bool getConnectionState();

    //! return ready state of the sensor
    bool isReadyForMeasurement();

    //!sensor stats
    QMap<QString,QString> getSensorStats();

    //!checks if sensor is busy
    bool isBusy();

protected:

    //! starts initialization
    bool initialize();

    //! move laser tracker to specified position
    bool move(double azimuth, double zenith, double distance,bool isrelativ);

    bool move(double x, double y, double z);

    //! sets laser tracke to home position
    bool home();

    //! turns motors on or off
    bool changeMotorState();

    //! toggle between frontside and backside
    bool toggleSightOrientation();

    //! compensation
    bool compensation() ;

private:
    QList<Reading*> measurePolar(MeasurementConfig *m);
    QList<Reading*> measureDistance(MeasurementConfig *m);
    QList<Reading*> measureDirection(MeasurementConfig *m);
    QList<Reading*> measureCartesian(MeasurementConfig *m);

    double myAzimuth;
    double myZenith;
    double myDistance;
    bool myMotor;
    bool myInit;
    bool myCompIt;
    int side;

    bool isConnected;

    //methodes to generate random value
    double randomX(int d, double m, double s);
    double randomNorm();
    double randomTriangular(double c, double a,double b);

    void noisyPolarReading(Reading *r);


};

#endif // P_PSEUDOTRACKER_H
