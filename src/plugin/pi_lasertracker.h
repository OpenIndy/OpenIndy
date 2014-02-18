#ifndef PI_LASERTRACKER_H
#define PI_LASERTRACKER_H

#include <QString>
#include <QDebug>

#include "sensor.h"
#include "reading.h"
#include "measurementconfig.h"

/*!
 * \brief The LaserTrackerInterface class
 * interface class for implementing laser tracker plugins (.dll).
 */

class LaserTracker : public Sensor
{

public:

    virtual ~LaserTracker(){}

    double sigmaAzimuth;
    double sigmaZenith;
    double sigmaDistance;

    bool accept(SensorControl* s, Configuration::SensorFunctionalities f){

        switch(f){
        case(Configuration::eMoveAngle):
            return this->move(s->az,s->ze,s->dist,s->isMoveRelativ);
        case(Configuration::eMoveXYZ):
            return this->move(s->x_,s->y_,s->z_);
        case(Configuration::eInitialize):
            return this->initialize();
        case(Configuration::eHome):
            return this->home();
        case(Configuration::eToggleSight):
            return this->toggleSightOrientation();
        case(Configuration::eMotorState):
            return this->changeMotorState(s->isMState);
        case(Configuration::eCompensation):
            return this->compensation();
        }
        return false;
    }

    //--------------has to be implemented-----------------------

    virtual QList<Configuration::ReadingTypes>* getSupportedReadingTypes() = 0;
    virtual QList<Configuration::ConnectionTypes>* getConnectionType() = 0;
    virtual QMap<QString,int>* getIntegerParameter() = 0;
    virtual QMap<QString,double>* getDoubleParameter() = 0;
    virtual QMap <QString, QStringList>* getStringParameter() = 0;


    //! laser tracker measures a point and returns an observation
    virtual QList<Reading*>measure(MeasurementConfig *mc) = 0;

    virtual void dataStream() = 0;
    virtual void sendCommandString(QString) = 0;

    //! connect app with laser tracker
    virtual bool connectSensor(ConnectionConfig*) = 0;

    //! disconnect app with laser tracker
    virtual bool disconnectSensor() = 0;

    //! checks if the measurementconfig is vaild
    virtual bool checkMeasurementConfig(MeasurementConfig*)=0;

    virtual PluginMetaData* getMetaData() = 0;

protected:

    //! starts initialization
    virtual bool initialize() = 0;

    //! move laser tracker to specified position
    virtual bool move(double azimuth, double zenith, double distance,bool isrelativ) = 0;

    virtual bool move(double x, double y, double z) = 0;

    //! sets laser tracke to home position
    virtual bool home() = 0;

    //! turns motors on or off
    virtual bool changeMotorState(bool state) = 0;

    //! toggle between frontside and backside
    virtual bool toggleSightOrientation() = 0;

    //! compensation
    virtual bool compensation() = 0;

};

#define LaserTracker_iidd "de.openIndy.Plugin.Sensor.LaserTracker.v001"

#endif // PI_LASERTRACKER_H
