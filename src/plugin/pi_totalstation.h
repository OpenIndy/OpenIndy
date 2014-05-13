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

    bool accept(SensorControl* s, Configuration::SensorFunctionalities f){

        switch(f){
        case(Configuration::eMoveAngle):
            return this->move(s->az,s->ze,s->dist,s->isMoveRelativ);
        case(Configuration::eToggleSight):
            return this->toggleSightOrientation();
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
    virtual QMap<QString,int>* getIntegerParameter();
    virtual QMap<QString,double>* getDoubleParameter();
    virtual QMap <QString, QStringList>* getStringParameter();
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

    //! connect app with total station
    virtual bool connectSensor(ConnectionConfig* connConfig) = 0;

    //! disconnect app with laser tracker
    virtual bool disconnectSensor() = 0;

    //! total station measures a point and returns a list of readings
    virtual QList<Reading*> measure(MeasurementConfig *mc) = 0;

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

     //! move totalstation to specified position
    virtual bool move(double azimuth, double zenith, double distance,bool isrelativ){this->writeToConsole("not available");}

    //! toggle between frontside and backside
    virtual bool toggleSightOrientation(){this->writeToConsole("not available");}


};

#define TotalStation_iidd "de.openIndy.Plugin.Sensor.TotalStation.v001"

#endif // PI_TOTALSTATION_H
