#include "sensorcontrol.h"
#include "function.h"
#include "sensorlistener.h"

#include "oimetadata.h"

SensorControl::SensorControl(Station *st)
{

    myStation = st;

    instrument = NULL;
    instrumentListener = new SensorListener(instrument);
    this->t = this->eNoStream;

    connect(this,SIGNAL(activateStatStream()),this->instrumentListener,SLOT(sensorStatStream()));
    connect(this,SIGNAL(activateReadingStream(int)),this->instrumentListener,SLOT(sensorReadingStream(int)));
    connect(instrumentListener,SIGNAL(connectionLost()),this,SLOT(streamLostSignal()));

    instrumentListener->moveToThread(&listenerThread);

    listenerThread.start();

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

    if(!this->sendDeactivateStream()){
        locker.unlock();
        emit commandFinished(false);
        return;
    }
    if(!instrument->isReadyForMeasurement()){
      locker.unlock();
      emit commandFinished(false);
      this->myEmitter.sendString("sensor not ready for measurement");
      return ;
    }

    if(!checkSensor()){
       locker.unlock();
       emit commandFinished(false);
       return;
    }

    MeasurementConfig mconfig = geom->getMeasurementConfig();
    QList<Reading*> readings = instrument->measure(&mconfig);

    if(readings.size() == 0){
        this->myEmitter.sendString("measurement not valid!");
        emit commandFinished(false);
        emit measurementFinished(geom->getId(),false);
    }else{
        this->storeReadings(readings,geom, isActiveCoordSys);
        emit commandFinished(true);
        emit measurementFinished(geom->getId(),true);
        emit this->recalcFeature(geom);
    }

    if(!this->sendActivateStream()){

    }

    locker.unlock();

}

void SensorControl::readingStream(int r){

    this->myEmitter.sendString("starting reading stream!");

    if(t == eReadingStream){
        return;
    }

    if(!this->sendDeactivateStream()){
        emit commandFinished(false);
        return;
    }

    typeOfReadingStream = r;

    this->t = this->eReadingStream;

    this->sendActivateStream();

}

void SensorControl::sensorStatsStream()
{
    this->myEmitter.sendString("starting stat stream!");

    if(t == eSenorStats){
        return;
    }

    if(!this->sendDeactivateStream()){
        emit commandFinished(false);
        return;
    }

    this->t = this->eSenorStats;

    this->sendActivateStream();
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

    if(!this->sendDeactivateStream()){
        locker.unlock();
        emit commandFinished(false);
        return;
    }

    if(!this->checkSensor()){
       locker.unlock();
       emit commandFinished(false);
       return;
    }

    this->myEmitter.sendString("start move command");

    this->az =azimuth;
    this->ze= zenith;
    this->dist = distance;
    this->isMoveRelativ = isRelative;

    bool wasSuccessful = instrument->accept(this, Configuration::eMoveAngle);

    emit commandFinished(wasSuccessful);

    this->sendActivateStream();

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

    if(!this->sendDeactivateStream()){
        locker.unlock();
        emit commandFinished(false);
        return;
    }

    if(!this->checkSensor()){
       locker.unlock();
       emit commandFinished(false);
       return;
    }

    this->myEmitter.sendString("start move command");


    this->x_ =x;
    this->y_ =y;
    this->z_ =z;

    bool wasSuccessful = instrument->accept(this, Configuration::eMoveXYZ);

    emit commandFinished(wasSuccessful);

    this->sendActivateStream();

    locker.unlock();
}

/*!
 * \brief SensorControl::initialize
 */
void SensorControl::initialize(){

    locker.lock();

    if(!this->checkSensor()){
       locker.unlock();
       emit commandFinished(false);
       return;
    }

    this->myEmitter.sendString("start initialize");

    bool wasSuccessful = instrument->accept(this, Configuration::eInitialize);

    emit commandFinished(wasSuccessful);

    locker.unlock();
}

/*!
 * \brief SensorControl::motorState
 */
void SensorControl::motorState(){

    locker.lock();

    if(!this->checkSensor()){
       locker.unlock();
       emit commandFinished(false);
       return;
    }

    this->myEmitter.sendString("changing motor state");

    bool wasSuccessful = instrument->accept(this, Configuration::eMotorState);

    emit commandFinished(wasSuccessful);

    locker.unlock();
}

/*!
 * \brief SensorControl::home
 */
void SensorControl::home(){

    locker.lock();

    if(!this->sendDeactivateStream()){
        locker.unlock();
        emit commandFinished(false);
        return;
    }

    if(!this->checkSensor()){
       locker.unlock();
       emit commandFinished(false);
       return;
    }

    this->myEmitter.sendString("set home");

    bool wasSuccessful = instrument->accept(this, Configuration::eHome);

    emit commandFinished(wasSuccessful);

    this->sendActivateStream();

    locker.unlock();
}

/*!
 * \brief SensorControl::toggleSight
 */
void SensorControl::toggleSight(){

    locker.lock();

    if(!this->sendDeactivateStream()){
        locker.unlock();
        emit commandFinished(false);
        return;
    }

    if(!this->checkSensor()){
       locker.unlock();
       emit commandFinished(false);
       return;
    }

    this->myEmitter.sendString("toggleSight");

    bool wasSuccessful =  instrument->accept(this, Configuration::eToggleSight);

    emit commandFinished(wasSuccessful);

    this->sendActivateStream();

    locker.unlock();
}

/*!
 * \brief SensorControl::compensation
 */
void SensorControl::compensation(){

    locker.lock();

    if(!this->checkSensor()){
       locker.unlock();
       emit commandFinished(false);
       return;
    }

    this->myEmitter.sendString("start compensation");

    bool wasSuccessful =  instrument->accept(this, Configuration::eCompensation);

    emit commandFinished(wasSuccessful);

    locker.unlock();
}

void SensorControl::doSelfDefinedAction(QString s)
{
    locker.lock();

    if(!this->checkSensor()){
       locker.unlock();
       emit commandFinished(false);
       return;
    }

    bool wasSuccessful =  instrument->doSelfDefinedAction(s);

    emit commandFinished(wasSuccessful);

    locker.unlock();

}

void SensorControl::stopReadingStream()
{
    this->sendDeactivateStream();
    this->t = this->eNoStream;
    this->myEmitter.sendString("reading stream stopped");
}

void SensorControl::stopStatStream()
{
    this->sendDeactivateStream();
    this->t = this->eNoStream;
    this->myEmitter.sendString("stat stream stopped");
}

void SensorControl::copyMe(SensorControl *sc)
{
    sc->instrument = this->instrument;
    sc->InstrumentConfig = this->InstrumentConfig;

    sc->instrumentListener = this->instrumentListener;
    connect(sc,SIGNAL(activateStatStream()),sc->instrumentListener,SLOT(sensorStatStream()));
    connect(sc,SIGNAL(activateReadingStream(int)),sc->instrumentListener,SLOT(sensorReadingStream(int)));
    connect(sc->instrumentListener,SIGNAL(connectionLost()),sc,SLOT(streamLostSignal()));

    sc->instrumentListener->moveToThread(&sc->listenerThread);

    sc->listenerThread.start();
}


/*!
 * \brief SensorControl::connect
 */
void SensorControl::connectSensor(ConnectionConfig *connConfig){

    locker.lock();


    if(this->t != eNoStream){
        this->sendDeactivateStream();
    }

    if(instrument->getConnectionState()){
        this->myEmitter.sendString("sensor already connected");

        if(this->t != eNoStream){
            this->sendActivateStream();
        }

        emit commandFinished(true);

        locker.unlock();

        return;
    }

    this->myEmitter.sendString("connecting sensor");

    bool wasSuccessful = instrument->connectSensor(connConfig);

    if(wasSuccessful){
       instrumentListener->setInstrument(instrument);
    }

    if(this->t != eNoStream){
        this->sendActivateStream();
    }


    emit commandFinished(wasSuccessful);

    locker.unlock();
}

/*!
 * \brief SensorControl::disconnect
 */
void SensorControl::disconnectSensor(){


    locker.lock();

    if(this->t != eNoStream){
        this->sendDeactivateStream();
    }

    this->myEmitter.sendString("disconnecting sensor");

    bool wasSuccessful = instrument->disconnectSensor();

    if(wasSuccessful){
        this->t = eNoStream;
    }

    emit commandFinished(wasSuccessful);

    locker.unlock();
}


void SensorControl::storeReadings(QList<Reading*>readings, Geometry* geom, bool isActiveCoordSys){

    for(int i = 0; i < readings.size();i++){
        this->saveReading(readings.at(i),geom,isActiveCoordSys);
    }

}

bool SensorControl::sendActivateStream()
{
    checkSensor();

    if(this->t != eNoStream){

       instrumentListener->isStreamActive = true;

       switch (t) {
       case eReadingStream:
           emit activateReadingStream(typeOfReadingStream);
           break;
       case eSenorStats:
           emit activateStatStream();
           break;
       default:
           break;
       }
    }


    return true;


}

bool SensorControl::sendDeactivateStream()
{
    if(!instrumentListener->isStreamActive || t == eNoStream){
        return true;
    }

    instrumentListener->isStreamActive = false;


    listenerThread.quit();

    if(!listenerThread.wait()){
       this->myEmitter.sendString("timeout - stream failed");
        return false;
    }

    listenerThread.start();

    return true;
}

bool SensorControl::checkSensor()
{
    if(this->instrumentListener->isStreamActive){
        this->myEmitter.sendString("sensor stream is active");
        return false;
    }

    if(instrument->getConnectionState()){

        if(instrument->isBusy()){
          this->myEmitter.sendString("sensor is busy");
          return false;
        }

    }else{
       this->myEmitter.sendString("sensor not connected");
        return false;
    }

    return true;
}

void SensorControl::streamLostSignal()
{
    this->t = eNoStream;
    this->instrumentListener->isStreamActive = false;
    this->myEmitter.sendString("lost sensor connection");
}

OiEmitter& SensorControl::getOiEmitter(){
    return this->myEmitter;
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

    switch(geom->getMeasurementConfig().typeOfReading){
        case(Configuration::ePolar) :{
            //set type
            r->typeofReading = Configuration::ePolar;
            //set accuracy
            r->rPolar.sigmaAzimuth = myStation->getInstrumentConfig().sigma.sigmaAzimuth;
            r->rPolar.sigmaZenith= myStation->getInstrumentConfig().sigma.sigmaZenith;
            r->rPolar.sigmaDistance = myStation->getInstrumentConfig().sigma.sigmaDistance;
            //store reading in station
            this->myStation->readingsPol.append(r);
            //create observation
            Observation *obs = new Observation(r,myStation,isActiveCoordSys);
            //save geometry in observation
            obs->myTargetGeometries.append(geom);
            //add observation to fit function of geom
            if(geom->getFunctions().size() > 0 && geom->getFunctions().at(0)->getMetaData()->iid == OiMetaData::iid_FitFunction){
                geom->getFunctions().at(0)->addObservation(obs, 0);
            }

            //save observation in geometry
            geom->addObservation(obs);
            //geom->insertReadingType(Configuration::ePolar,Configuration::sPolar);
            //save observation in station
            this->myStation->coordSys->addObservation(obs);

            break;
        }
        case(Configuration::eDistance) :{
            //set type
            r->typeofReading = Configuration::eDistance;
            //set accuracy
            r->rDistance.sigmaDistance = myStation->getInstrumentConfig().sigma.sigmaDistance;
            //store reading in station
            this->myStation->readingsDist.append(r);
            //create observation
            Observation *obs = new Observation(r, myStation,isActiveCoordSys);
            //save geometry in observation
            obs->myTargetGeometries.append(geom);
            //add observation to fit function of geom
            if(geom->getFunctions().size() > 0 && geom->getFunctions().at(0)->getMetaData()->iid == OiMetaData::iid_FitFunction){
                geom->getFunctions().at(0)->addObservation(obs, 0);
            }
            //save observation in geometry
            geom->addObservation(obs);
            //geom->insertReadingType(Configuration::eDistance,Configuration::sDistance);
            //save observation in station
            this->myStation->coordSys->addObservation(obs);

            break;
        }
        case(Configuration::eDirection) :{
            //set type
            r->typeofReading =Configuration::eDirection;
            //set accuracy
            r->rDirection.sigmaAzimuth = myStation->getInstrumentConfig().sigma.sigmaAzimuth;
            r->rDirection.sigmaZenith= myStation->getInstrumentConfig().sigma.sigmaZenith;
            //store reading in station
            this->myStation->readingsDir.append(r);
            //create observation
            Observation *obs = new Observation(r, myStation,isActiveCoordSys);
            //save geometry in observation
            obs->myTargetGeometries.append(geom);
            //add observation to fit function of geom
            if(geom->getFunctions().size() > 0 && geom->getFunctions().at(0)->getMetaData()->iid == OiMetaData::iid_FitFunction){
                geom->getFunctions().at(0)->addObservation(obs, 0);
            }
            //save observation in geometry
            geom->addObservation(obs);
            //geom->insertReadingType(Configuration::eDirection,Configuration::sDirection);
            //save observation in station
            this->myStation->coordSys->addObservation(obs);

            break;
        }
        case(Configuration::eCartesian) :{
            //set type
            r->typeofReading = Configuration::eCartesian;
            //set accuracy
            r->rCartesian.sigmaXyz = myStation->getInstrumentConfig().sigma.sigmaXyz;
            //store reading in station
            this->myStation->readingsXyz.append(r);
            //create observation
            Observation *obs = new Observation(r, myStation,isActiveCoordSys);
            //save geometry in observation
            //add observation to fit function of geom
            if(geom->getFunctions().size() > 0 && geom->getFunctions().at(0)->getMetaData()->iid == OiMetaData::iid_FitFunction){
                geom->getFunctions().at(0)->addObservation(obs, 0);
            }
            obs->myTargetGeometries.append(geom);
            //save observation in geometry
            geom->addObservation(obs);
            //geom->insertReadingType(Configuration::eCartesian,Configuration::sCartesian);
            //save observation in station
            this->myStation->coordSys->addObservation(obs);

            break;
        }
        case(Configuration::eLevel) :{
            //set type
            r->typeofReading = Configuration::eLevel;
            //store reading in station
            this->myStation->readingsLevel.append(r);
            //create observation
            Observation *obs = new Observation(r, myStation,isActiveCoordSys);
            //save geometry in observation
            //add observation to fit function of geom
            if(geom->getFunctions().size() > 0 && geom->getFunctions().at(0)->getMetaData()->iid == OiMetaData::iid_FitFunction){
                geom->getFunctions().at(0)->addObservation(obs, 0);
            }
            obs->myTargetGeometries.append(geom);
            //save observation in geometry
            geom->addObservation(obs);
            //geom->insertReadingType(Configuration::eLevel,Configuration::sLevel);
            //save observation in station
            this->myStation->coordSys->addObservation(obs);

        break;
    }
        case(Configuration::eTemperatur) :{
            //set type
            r->typeofReading = Configuration::eTemperatur;
            //store reading in station
            this->myStation->readingsTemperatur.append(r);
            //create observation
            Observation *obs = new Observation(r, myStation,isActiveCoordSys);
            //save geometry in observation
            //add observation to fit function of geom
            if(geom->getFunctions().size() > 0 && geom->getFunctions().at(0)->getMetaData()->iid == OiMetaData::iid_FitFunction){
                geom->getFunctions().at(0)->addObservation(obs, 0);
            }
            obs->myTargetGeometries.append(geom);
            //save observation in geometry
            geom->addObservation(obs);
            //geom->insertReadingType(Configuration::eTemperatur,Configuration::sTemperatur);
            //save observation in station
            this->myStation->coordSys->addObservation(obs);

        break;
        }
        case(Configuration::eUndefined) :{
                //set type
                r->typeofReading = Configuration::eUndefined;
                //store reading in station
                this->myStation->readingsUndefined.append(r);
                //create observation
                Observation *obs = new Observation(r, myStation,isActiveCoordSys);
                //save geometry in observation
                //add observation to fit function of geom
                if(geom->getFunctions().size() > 0 && geom->getFunctions().at(0)->getMetaData()->iid == OiMetaData::iid_FitFunction){
                    geom->getFunctions().at(0)->addObservation(obs, 0);
                }
                obs->myTargetGeometries.append(geom);
                //save observation in geometry
                geom->addObservation(obs);
                //geom->insertReadingType(Configuration::eUndefined, instrument->getUndefinedReadingName());
                //save observation in station
                this->myStation->coordSys->addObservation(obs);

        break;
        }
    }

    r->makeBackup();

}

