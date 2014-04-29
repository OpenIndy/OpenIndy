#ifndef P_PSEUDOTRACKER_H
#define P_PSEUDOTRACKER_H

#include <QtGlobal>
#include <QDateTime>
#include <QObject>
#include <QDebug>
#include <QThread>
#include <QFile>
#include <QStringList>
#include "pi_lasertracker.h"


class PseudoTracker : public LaserTracker
{
public:

    PseudoTracker();

    virtual QList<Configuration::ReadingTypes>* getSupportedReadingTypes();

    virtual QList<Configuration::ConnectionTypes>* getConnectionType();

    virtual QMap<QString,int>* getIntegerParameter();

    virtual QMap<QString,double>* getDoubleParameter();

    virtual QMap <QString, QStringList>* getStringParameter();

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

    //! laser tracker measures a point and returns an observation
    QList<Reading*> measure(MeasurementConfig *mc);

    void dataStream() ;

    void sendCommandString(QString);

    //! checks if the measurementconfig is vaild
    bool checkMeasurementConfig(MeasurementConfig*);

    //! connect app with laser tracker
    bool connectSensor(ConnectionConfig*);

    //! disconnect app with laser tracker
    bool disconnectSensor();

    PluginMetaData* getMetaData();

protected:

    //! starts initialization
    bool initialize();

    //! move laser tracker to specified position
    bool move(double azimuth, double zenith, double distance,bool isrelativ);

    bool move(double x, double y, double z);

    //! sets laser tracke to home position
    bool home();

    //! turns motors on or off
    bool changeMotorState(bool state);

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

};

#endif // P_PSEUDOTRACKER_H
