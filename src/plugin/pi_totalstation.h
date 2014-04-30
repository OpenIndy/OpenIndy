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
    virtual QList<Configuration::ConnectionTypes>* getConnectionType() = 0;

    //get meta data
    virtual PluginMetaData* getMetaData() = 0;

    //individually defined sensor parameter
    virtual QMap<QString,int>* getIntegerParameter() = 0;
    virtual QMap<QString,double>* getDoubleParameter() = 0;
    virtual QMap <QString, QStringList>* getStringParameter() = 0;

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

    //! connect app with total station
    virtual bool connectSensor(ConnectionConfig* connConfig) = 0;

    //! disconnect app with laser tracker
    virtual bool disconnectSensor() = 0;

    //! total station measures a point and returns a list of readings
    virtual QList<Reading*> measure(MeasurementConfig *mc) = 0;

    //stream
    virtual void dataStream() = 0;

    //exec individually defined command strings
    virtual void sendCommandString(QString cmd) = 0;

    //! checks if the measurementconfig is vaild
    virtual bool checkMeasurementConfig(MeasurementConfig* mc)=0;

protected:

     //! move totalstation to specified position
    virtual bool move(double azimuth, double zenith, double distance,bool isrelativ) = 0;

    //! toggle between frontside and backside
    virtual bool toggleSightOrientation() = 0;


};

#define TotalStation_iidd "de.openIndy.Plugin.Sensor.TotalStation.v001"

#endif // PI_TOTALSTATION_H
