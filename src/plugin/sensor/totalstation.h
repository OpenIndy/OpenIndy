#ifndef TOTALSTATION_H
#define TOTALSTATION_H

#include "sensor.h"

/*!
 * \brief The TotalStation class
 */
class TotalStation : public Sensor
{
    Q_OBJECT

public:
    TotalStation(QObject *parent = 0) : Sensor(parent){}

    virtual ~TotalStation(){}

    //##########################################
    //calls the appropriate laser tracker method
    //##########################################

    bool accept(const SensorFunctions &method, const SensorAttributes &sAttr){

        switch(method){
        case(eMoveAngle):
            return this->move(sAttr.moveAzimuth, sAttr.moveZenith, sAttr.moveDistance, sAttr.moveIsRelative);
        case(eMoveXYZ):
            return this->move(sAttr.moveX, sAttr.moveY, sAttr.moveZ);
        case(eToggleSight):
            return this->toggleSightOrientation();
        }
        return false;

    }

protected:

    //#####################
    //total station methods
    //#####################

    /*!
     * \brief move
     * \param azimuth
     * \param zenith
     * \param distance
     * \param isrelativ
     * \return
     */
    virtual bool move(const double &azimuth, const double &zenith, const double &distance, const bool &isRelative){
        emit this->sensorMessage(QString("Total Station not implemented correctly: move method missing"));
        return false;
    }

    /*!
     * \brief move
     * \param x
     * \param y
     * \param z
     * \return
     */
    virtual bool move(const double &x, const double &y, const double &z){
        emit this->sensorMessage(QString("Total Station not implemented correctly: move method missing"));
        return false;
    }

    /*!
     * \brief toggleSightOrientation
     * \return
     */
    virtual bool toggleSightOrientation(){
        emit this->sensorMessage(QString("Total Station not implemented correctly: toggle sight method missing"));
        return false;
    }


};

#define TotalStation_iidd "de.openIndy.plugin.sensor.totalStation.v001"

#endif // TOTALSTATION_H
