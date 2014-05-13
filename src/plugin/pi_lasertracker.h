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
            return this->changeMotorState();
        case(Configuration::eCompensation):
            return this->compensation();
        }
        return false;
    }

    //--------------has to be implemented-----------------------

    //-----get sensor capabilities-----

    virtual QList<Configuration::ReadingTypes>* getSupportedReadingTypes() = 0;
    virtual QList<Configuration::SensorFunctionalities> getSupportedSensorActions() = 0;
    virtual QList<Configuration::ConnectionTypes>* getConnectionType() = 0;

    //get meta data
    virtual PluginMetaData* getMetaData() = 0;

    //individually defined sensor parameter
    virtual QMap<QString,int>* getIntegerParameter() = 0;
    virtual QMap<QString,double>* getDoubleParameter() = 0;
    virtual QMap <QString, QStringList>* getStringParameter() = 0;
    virtual QStringList selfDefinedActions();
    virtual bool doSelfDefinedAction(QString a);

    /*default accuracy
     *keys:
     * sigmaAzimuth sigmaAzimuth sigmaDistance
     * sigmaXyz
     * sigmaTempDeg
     * sigmaAngleXZ
     * sigmaAngleYZ
     */
    virtual QMap<QString,double>* getDefaultAccuracy() = 0;

    //-----sensor actions-----

    //! abort a running action
    virtual void abortAction() = 0;

    //! connect app with laser tracker
    virtual bool connectSensor(ConnectionConfig*) = 0;

    //! disconnect app with laser tracker
    virtual bool disconnectSensor() = 0;

    //! laser tracker measures a point and returns a list of readings
    virtual QList<Reading*>measure(MeasurementConfig *mc) = 0;

    //! stream
    virtual QVariantMap readingStream(Configuration::ReadingTypes streamFormat) = 0;

    //! getConnectionState
    virtual bool getConnectionState() = 0;

    //! return ready state of the sensor
    virtual bool isReady() = 0;

    //!sensor stats
    virtual QMap<QString,QString> getSensorStats()=0;

    //!checks if sensor is busy
    virtual bool isBusy() = 0;


protected:

    //! starts initialization
    virtual bool initialize(){this->writeToConsole("not available");}

    //! move laser tracker to specified position
    virtual bool move(double azimuth, double zenith, double distance,bool isrelativ){this->writeToConsole("not available");}

    virtual bool move(double x, double y, double z){this->writeToConsole("not available");}

    //! sets laser tracke to home position
    virtual bool home(){this->writeToConsole("not available");}

    //! turns motors on or off
    virtual bool changeMotorState(){this->writeToConsole("not available");}

    //! toggle between frontside and backside
    virtual bool toggleSightOrientation(){this->writeToConsole("not available");}

    //! compensation
    virtual bool compensation(){this->writeToConsole("not available");}

};

#define LaserTracker_iidd "de.openIndy.Plugin.Sensor.LaserTracker.v001"

#endif // PI_LASERTRACKER_H
