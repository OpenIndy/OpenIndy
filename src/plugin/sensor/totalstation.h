#ifndef PI_TOTALSTATION_H
#define PI_TOTALSTATION_H

#include "sensor.h"
#include "measurementconfig.h"
#include <QString>
#include "reading.h"


/*!
 * \brief The TotalStation class
 * Interface for implementing totalstation plugins
 */

class TotalStation : public Sensor
{

public:
    virtual ~TotalStation(){}

    bool accept(SensorControl* s, SensorFunctions f){

        /*switch(f){
        case(eMoveAngle):
            return this->move(s->az,s->ze,s->dist,s->isMoveRelativ);
        case(eMoveXYZ):
            return this->move(s->x_, s->y_, s->z_);
        case(eToggleSight):
            return this->toggleSightOrientation();
        }*/
        return false;
    }

    //--------------has to be implemented-----------------------

    //-----get sensor capabilities-----

    virtual QList<ReadingTypes>* getSupportedReadingTypes() const = 0;
    virtual QList<SensorFunctions> getSupportedSensorActions() const = 0;
    virtual QList<ConnectionTypes>* getConnectionType() const = 0;

    //get meta data
    virtual PluginMetaData* getMetaData() const = 0;

    //individually defined sensor parameter
    virtual QMap<QString,int>* getIntegerParameter() const = 0;
    virtual QMap<QString,double>* getDoubleParameter() const = 0;
    virtual QMap <QString, QStringList>* getStringParameter() const = 0;
    virtual QStringList selfDefinedActions() const = 0;
    virtual bool doSelfDefinedAction(QString a) = 0;

    /*default accuracy
     *keys:
     * sigmaAzimuth sigmaAzimuth sigmaDistance
     * sigmaXyz
     * sigmaTempDeg
     * sigmaAngleXZ
     * sigmaAngleYZ
     */
    virtual QMap<QString,double>* getDefaultAccuracy() const = 0;

    //-----sensor actions-----
    //! abort a running action
    virtual void abortAction() = 0;

    //! connect app with total station
    virtual bool connectSensor(ConnectionConfig* connConfig) = 0;

    //! disconnect app with laser tracker
    virtual bool disconnectSensor() = 0;

    //! total station measures a point and returns a list of readings
    virtual QList<Reading*> measure(MeasurementConfig *mc) = 0;

    //! stream
    virtual QVariantMap readingStream(ReadingTypes streamFormat) = 0;

    //! getConnectionState
    virtual bool getConnectionState() = 0;

    //! return ready state of the sensor
    virtual bool isReadyForMeasurement() = 0;

    //!sensor stats
    virtual QMap<QString,QString> getSensorStats()=0;

    //!checks if sensor is busy
    virtual bool isBusy() = 0;

protected:

     //! move totalstation to specified position
    virtual bool move(double azimuth, double zenith, double distance,bool isrelativ){this->writeToConsole("not available");return false;}

    //! move total station to specified xyz position
    virtual bool move(double x, double y, double z){this->writeToConsole("not available");return false;}

    //! toggle between frontside and backside
    virtual bool toggleSightOrientation(){this->writeToConsole("not available");return false;}


};

#define TotalStation_iidd "de.openIndy.Plugin.Sensor.TotalStation.v001"

#endif // PI_TOTALSTATION_H
