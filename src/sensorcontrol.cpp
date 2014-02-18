#include "sensorcontrol.h"
#include "function.h"

#include "oimetadata.h"

SensorControl::SensorControl(Station *st)
{

    myStation = st;

}

SensorControl::~SensorControl()
{

}

/*!
 * \brief SensorControl::measure
 * \param mConfig
 * \param geom
 */
void SensorControl::measure(Geometry* geom,bool isActiveCoordSys){

    locker.lock();

    QList<Reading*> readings = myStation->instrument->measure(&(geom->mConfig));

    qDebug() << "count readings" << readings.size();
    if(readings.size() == 0){
        Console::addLine("measurement not valid!");
        emit commandFinished(false);
    }else{
        this->storeReadings(readings,geom, isActiveCoordSys);
        emit commandFinished(true);
        emit this->recalcFeature(geom);
    }

    locker.unlock();

}

void SensorControl::stream(){
    myStation->instrument->dataStream();
}

/*!
 * \brief sendCommandString
 * \param command
 */
void SensorControl::sendCommandString(QString command){
    myStation->instrument->sendCommandString(command);
}

/*!
 * \brief SensorControl::move
 * \param azimuth
 * \param zenith
 * \param distance
 * \param isRelative
 */
void SensorControl::move(double azimuth, double zenith, double distance, bool isRelative){

    locker.lock();

    this->az =azimuth;
    this->ze= zenith;
    this->dist = distance;
    this->isMoveRelativ = isRelative;

    bool wasSuccessful = myStation->instrument->accept(this, Configuration::eMoveAngle);

    emit commandFinished(wasSuccessful);

    locker.unlock();

}

/*!
 * \brief SensorControl::move
 * \param x
 * \param y
 * \param z
 */
void SensorControl::move(double x, double y, double z){

    locker.lock();

    this->x_ =x;
    this->y_ =y;
    this->z_ =z;

    bool wasSuccessful = myStation->instrument->accept(this, Configuration::eMoveXYZ);

    emit commandFinished(wasSuccessful);

    locker.unlock();
}

/*!
 * \brief SensorControl::initialize
 */
void SensorControl::initialize(){

    locker.lock();

    bool wasSuccessful = myStation->instrument->accept(this, Configuration::eInitialize);

    emit commandFinished(wasSuccessful);

    locker.unlock();
}

/*!
 * \brief SensorControl::motorState
 */
void SensorControl::motorState(bool b){

    locker.lock();

    this->motorState(b);

    bool wasSuccessful = myStation->instrument->accept(this, Configuration::eMotorState);

    emit commandFinished(wasSuccessful);

    locker.unlock();
}

/*!
 * \brief SensorControl::home
 */
void SensorControl::home(){

    locker.lock();

    bool wasSuccessful = myStation->instrument->accept(this, Configuration::eHome);

    emit commandFinished(wasSuccessful);

    locker.unlock();
}

/*!
 * \brief SensorControl::toggleSight
 */
void SensorControl::toggleSight(){

    locker.lock();
    bool wasSuccessful =  myStation->instrument->accept(this, Configuration::eToggleSight);

    emit commandFinished(wasSuccessful);

    locker.unlock();
}

/*!
 * \brief SensorControl::compensation
 */
void SensorControl::compensation(){

    locker.lock();
    bool wasSuccessful =  myStation->instrument->accept(this, Configuration::eCompensation);

    emit commandFinished(wasSuccessful);

    locker.unlock();
}

/*!
 * \brief SensorControl::connect
 */
void SensorControl::connectSensor(ConnectionConfig *connConfig){

    locker.lock();

    bool wasSuccessful = myStation->instrument->connectSensor(connConfig);
    myStation->instrument->isConnected = wasSuccessful;
    emit commandFinished(wasSuccessful);

    locker.unlock();
}

/*!
 * \brief SensorControl::disconnect
 */
void SensorControl::disconnectSensor(){

    locker.lock();

    bool wasSuccessful = myStation->instrument->disconnectSensor();

    if (wasSuccessful){
            myStation->instrument->isConnected = false;
    }

    emit commandFinished(wasSuccessful);

    locker.unlock();
}


    void SensorControl::storeReadings(QList<Reading*>readings, Geometry* geom, bool isActiveCoordSys){

        for(int i = 0; i < readings.size();i++){
            this->saveReading(readings.at(i),geom,isActiveCoordSys);
        }

    }

/*!
 * \brief SensorControl::saveReading
 * \param r
 * \param mConfig
 * \param geom
 *
 * transforms the reading into an observation and saves the observation in the station and
 * given geometry
 */
void SensorControl::saveReading(Reading* r, Geometry* geom, bool isActiveCoordSys){

    r->id = Configuration::generateID();
    r->measuredAt = QDateTime::currentDateTime();

    switch(geom->mConfig.typeOfReading){
        case(Configuration::ePolar) :{
            //set type
            r->typeofReading = Configuration::ePolar;
            //set accuracy
            r->rPolar.sigmaAzimuth = myStation->getInstrumentConfig()->sigma.sigmaAzimuth;
            r->rPolar.sigmaZenith= myStation->getInstrumentConfig()->sigma.sigmaZenith;
            r->rPolar.sigmaDistance = myStation->getInstrumentConfig()->sigma.sigmaDistance;
            //store reading in station
            this->myStation->readingsPol.append(r);
            //create observation
            Observation *obs = new Observation(r,myStation);
            //check if the active coordSys is the station sys
            if(isActiveCoordSys){
                obs->isValid = true;
            }else{
                obs->isValid = false;
            }
            //calc obs Xyz
            obs->myOriginalXyz = Reading::toCartesian(r->rPolar.azimuth,r->rPolar.zenith,r->rPolar.distance);
            obs->myXyz = Reading::toCartesian(r->rPolar.azimuth,r->rPolar.zenith,r->rPolar.distance);
            //calc sigma xyz
            obs->sigmaXyz = r->errorPropagationPolarToCart();
            //save geometry in observation
            obs->myTargetGeometries.append(geom);
            //add observation to fit function of geom
            if(geom->functionList.size() > 0 && geom->functionList.at(0)->getMetaData()->iid == OiMetaData::iid_FitFunction){
                geom->functionList.at(0)->addObservation(obs, 0);
            }

            //save observation in geometry
            geom->myObservations.append(obs);
            geom->insertReadingType(Configuration::ePolar,Configuration::sPolar);
            //save observation in station
            this->myStation->coordSys->observations.append(obs);

            break;
        }
        case(Configuration::eDistance) :{
            //set type
            r->typeofReading = Configuration::eDistance;
            //set accuracy
            r->rDistance.sigmaDistance = myStation->getInstrumentConfig()->sigma.sigmaDistance;
            //store reading in station
            this->myStation->readingsDist.append(r);
            //create observation
            Observation *obs = new Observation(r, myStation);
            //check if the active coordSys is the station sys
            obs->isValid = false;
            //save geometry in observation
            obs->myTargetGeometries.append(geom);
            //add observation to fit function of geom
            if(geom->functionList.size() > 0 && geom->functionList.at(0)->getMetaData()->iid == OiMetaData::iid_FitFunction){
                geom->functionList.at(0)->addObservation(obs, 0);
            }
            //save observation in geometry
            geom->myObservations.append(obs);
            geom->insertReadingType(Configuration::eDistance,Configuration::sDistance);
            //save observation in station
            this->myStation->coordSys->observations.append(obs);

            break;
        }
        case(Configuration::eDirection) :{
            //set type
            r->typeofReading =Configuration::eDirection;
            //set accuracy
            r->rDirection.sigmaAzimuth = myStation->getInstrumentConfig()->sigma.sigmaAzimuth;
            r->rDirection.sigmaZenith= myStation->getInstrumentConfig()->sigma.sigmaZenith;
            //store reading in station
            this->myStation->readingsDir.append(r);
            //create observation
            Observation *obs = new Observation(r, myStation);
            //check if the active coordSys is the station sys
            obs->isValid = false;
            //save geometry in observation
            obs->myTargetGeometries.append(geom);
            //add observation to fit function of geom
            if(geom->functionList.size() > 0 && geom->functionList.at(0)->getMetaData()->iid == OiMetaData::iid_FitFunction){
                geom->functionList.at(0)->addObservation(obs, 0);
            }
            //save observation in geometry
            geom->myObservations.append(obs);
            geom->insertReadingType(Configuration::eDirection,Configuration::sDirection);
            //save observation in station
            this->myStation->coordSys->observations.append(obs);

            break;
        }
        case(Configuration::eCartesian) :{
            //set type
            r->typeofReading = Configuration::eCartesian;
            //set accuracy
            r->rCartesian.sigmaXyz = myStation->getInstrumentConfig()->sigma.sigmaXyz;
            //store reading in station
            this->myStation->readingsXyz.append(r);
            //create observation
            Observation *obs = new Observation(r, myStation);
            //check if the active coordSys is the station sys
            if(isActiveCoordSys){
                obs->isValid = true;
            }else{
                obs->isValid = false;
            }
            //save geometry in observation
            //add observation to fit function of geom
            if(geom->functionList.size() > 0 && geom->functionList.at(0)->getMetaData()->iid == OiMetaData::iid_FitFunction){
                geom->functionList.at(0)->addObservation(obs, 0);
            }
            obs->myTargetGeometries.append(geom);
            //save observation in geometry
            geom->myObservations.append(obs);
            geom->insertReadingType(Configuration::eCartesian,Configuration::sCartesian);
            //save observation in station
            this->myStation->coordSys->observations.append(obs);

            break;
        }
        case(Configuration::eLevel) :{
            //set type
            r->typeofReading = Configuration::eLevel;
            //store reading in station
            this->myStation->readingsXyz.append(r);
            //create observation
            Observation *obs = new Observation(r, myStation);
            //check if the active coordSys is the station sys
            obs->isValid = false;
            //save geometry in observation
            //add observation to fit function of geom
            if(geom->functionList.size() > 0 && geom->functionList.at(0)->getMetaData()->iid == OiMetaData::iid_FitFunction){
                geom->functionList.at(0)->addObservation(obs, 0);
            }
            obs->myTargetGeometries.append(geom);
            //save observation in geometry
            geom->myObservations.append(obs);
            geom->insertReadingType(Configuration::eLevel,Configuration::sLevel);
            //save observation in station
            this->myStation->coordSys->observations.append(obs);

        break;
    }
    case(Configuration::eUndefined) :{
            //set type
            r->typeofReading = Configuration::eUndefined;
            //store reading in station
            this->myStation->readingsXyz.append(r);
            //create observation
            Observation *obs = new Observation(r, myStation);
            //check if the active coordSys is the station sys
            obs->isValid = false;
            //save geometry in observation
            //add observation to fit function of geom
            if(geom->functionList.size() > 0 && geom->functionList.at(0)->getMetaData()->iid == OiMetaData::iid_FitFunction){
                geom->functionList.at(0)->addObservation(obs, 0);
            }
            obs->myTargetGeometries.append(geom);
            //save observation in geometry
            geom->myObservations.append(obs);
            geom->insertReadingType(Configuration::eUndefined, this->myStation->instrument->getUndefinedReadingName());
            //save observation in station
            this->myStation->coordSys->observations.append(obs);

    break;
    }
    }


}
