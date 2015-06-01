#include "sensorcontrol.h"

//#include "function.h"
#include "sensorlistener.h"
//#include "oimetadata.h"
/*
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

    bool wasSuccessful = instrument->accept(this, eMoveAngle);

    emit commandFinished(wasSuccessful);

    this->sendActivateStream();

    locker.unlock();

}

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

    bool wasSuccessful = instrument->accept(this, eMoveXYZ);

    emit commandFinished(wasSuccessful);

    this->sendActivateStream();

    locker.unlock();
}

void SensorControl::initialize(){

    locker.lock();

    if(!this->checkSensor()){
       locker.unlock();
       emit commandFinished(false);
       return;
    }

    this->myEmitter.sendString("start initialize");

    bool wasSuccessful = instrument->accept(this, eInitialize);

    emit commandFinished(wasSuccessful);

    locker.unlock();
}

void SensorControl::motorState(){

    locker.lock();

    if(!this->checkSensor()){
       locker.unlock();
       emit commandFinished(false);
       return;
    }

    this->myEmitter.sendString("changing motor state");

    bool wasSuccessful = instrument->accept(this, eMotorState);

    emit commandFinished(wasSuccessful);

    locker.unlock();
}

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

    bool wasSuccessful = instrument->accept(this, eHome);

    emit commandFinished(wasSuccessful);

    this->sendActivateStream();

    locker.unlock();
}

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

    bool wasSuccessful =  instrument->accept(this, eToggleSight);

    emit commandFinished(wasSuccessful);

    this->sendActivateStream();

    locker.unlock();
}

void SensorControl::compensation(){

    locker.lock();

    if(!this->checkSensor()){
       locker.unlock();
       emit commandFinished(false);
       return;
    }

    this->myEmitter.sendString("start compensation");

    bool wasSuccessful =  instrument->accept(this, eCompensation);

    emit commandFinished(wasSuccessful);

    locker.unlock();
}

void SensorControl::doSelfDefinedAction(QString s)
{
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

    bool wasSuccessful = instrument->doSelfDefinedAction(s);

    emit commandFinished(wasSuccessful);

    if(!this->sendActivateStream()){

    }

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

    sc->instrumentListener->setInstrument(this->instrument);
    sc->instrumentListener = this->instrumentListener;
    connect(sc,SIGNAL(activateStatStream()),sc->instrumentListener,SLOT(sensorStatStream()));
    connect(sc,SIGNAL(activateReadingStream(int)),sc->instrumentListener,SLOT(sensorReadingStream(int)));
    connect(sc->instrumentListener,SIGNAL(connectionLost()),sc,SLOT(streamLostSignal()));

    sc->instrumentListener->moveToThread(&sc->listenerThread);

    sc->listenerThread.start();
}

void SensorControl::addReading(Reading *r, Geometry *geom, bool isActiveCoordSys)
{
    this->saveReading(r, geom, isActiveCoordSys);
    emit commandFinished(true);
    emit measurementFinished(geom->getId(),true);
    emit this->recalcFeature(geom);
}

ReadingTypes SensorControl::getTypeOfReadingStream(){

    return (ReadingTypes) this->typeOfReadingStream;

}

void SensorControl::connectSensor(ConnectionConfig *connConfig){

    locker.lock();

    listenerThread.start();

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

    listenerThread.quit();

    if(!listenerThread.wait()){
        this->myEmitter.sendString("timeout - stream failed");
        emit commandFinished(false);
    }

    emit commandFinished(wasSuccessful);

    locker.unlock();
}


void SensorControl::storeReadings(QList<Reading*>readings, Geometry* geom, bool isActiveCoordSys){

    for(int i = 0; i < readings.size();i++){

        if(readings.at(i) != NULL){
            this->saveReading(readings.at(i),geom,isActiveCoordSys);
        }
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

 //transforms the reading into an observation and saves the observation in the station and
 //given geometry
void SensorControl::saveReading(Reading* r, Geometry* geom, bool isActiveCoordSys){

    //r->id = generateID();

    switch(geom->getMeasurementConfig().typeOfReading){
        case(ePolarReading) :{
            //set type
            r->typeOfReading = ePolarReading;
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
            //geom->insertReadingType(ePolar,sPolar);
            //save observation in station
            this->myStation->coordSys->addObservation(obs);

            break;
        }
        case(eDistanceReading) :{
            //set type
            r->typeOfReading = eDistanceReading;
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
            //geom->insertReadingType(eDistance,sDistance);
            //save observation in station
            this->myStation->coordSys->addObservation(obs);

            break;
        }
        case(eDirectionReading) :{
            //set type
            r->typeOfReading =eDirectionReading;
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
            //geom->insertReadingType(eDirection,sDirection);
            //save observation in station
            this->myStation->coordSys->addObservation(obs);

            break;
        }
        case(eCartesianReading) :{
            //set type
            r->typeOfReading = eCartesianReading;
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
            //geom->insertReadingType(eCartesian,sCartesian);
            //save observation in station
            this->myStation->coordSys->addObservation(obs);

            break;
        }
        case(eLevelReading) :{
            //set type
            r->typeOfReading = eLevelReading;
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
            //geom->insertReadingType(eLevel,sLevel);
            //save observation in station
            this->myStation->coordSys->addObservation(obs);

        break;
    }
        case(eTemperatureReading) :{
            //set type
            r->typeOfReading = eTemperatureReading;
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
            //geom->insertReadingType(eTemperatur,sTemperatur);
            //save observation in station
            this->myStation->coordSys->addObservation(obs);

        break;
        }
        case(eUndefinedReading) :{
                //set type
                r->typeOfReading = eUndefinedReading;
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
                //geom->insertReadingType(eUndefined, instrument->getUndefinedReadingName());
                //save observation in station
                this->myStation->coordSys->addObservation(obs);

        break;
        }
    }

    r->makeBackup();

}
*/

/*!
 * \brief SensorControl::SensorControl
 * \param station
 * \param parent
 */
SensorControl::SensorControl(QPointer<Station> &station, QObject *parent) : QObject(parent), station(station){

    //create sensor listener, connect it and move it to thread
    this->sensorListener = new SensorListener(this->locker);
    QObject::connect(&this->listenerThread, &QThread::started, this->sensorListener.data(), &SensorListener::startStreaming, Qt::AutoConnection);

    //connect(this,SIGNAL(activateStatStream()),this->sensorListener,SLOT(sensorStatStream()));
    //connect(this,SIGNAL(activateReadingStream(int)),this->sensorListener,SLOT(sensorReadingStream(int)));
    //connect(sensorListener,SIGNAL(connectionLost()),this,SLOT(streamLostSignal()));
    //sensorListener->moveToThread(&listenerThread);
    //listenerThread.start();

}

/*!
 * \brief SensorControl::~SensorControl
 */
SensorControl::~SensorControl(){

    //stop listener thread if it is still running
    if(this->listenerThread.isRunning()){
        this->listenerThread.quit();
        this->listenerThread.wait();
    }

    delete this->sensorListener;

}

/*!
 * \brief SensorControl::getSensor
 * \return
 */
const QPointer<Sensor> &SensorControl::getSensor() const{
    return this->sensor;
}

/*!
 * \brief SensorControl::setSensor
 * \param sensor
 */
void SensorControl::setSensor(const QPointer<Sensor> &sensor){

    //check sensor
    if(sensor.isNull()){
        return;
    }

    //check old sensor and add it to list of used sensors
    if(!this->sensor.isNull()){

        //stop listener thread if it is still running
        if(this->listenerThread.isRunning()){
            this->listenerThread.quit();
            this->listenerThread.wait(5000);
        }

        this->usedSensors.append(this->sensor);

    }

    //set new sensor
    this->sensor = sensor;
    this->sensorListener->setSensor(this->sensor);

    //move sensor listener to thread and start listening
    this->sensorListener->moveToThread(&this->listenerThread);
    this->listenerThread.start();

}

/*!
 * \brief SensorControl::getUsedSensors
 * \return
 */
const QList<QPointer<Sensor> > &SensorControl::getUsedSensors() const{
    return this->usedSensors;
}

/*!
 * \brief SensorControl::getSensorListener
 * \return
 */
const QPointer<const SensorListener> SensorControl::getSensorListener() const{
    return this->sensorListener;
}

/*!
 * \brief SensorControl::setStreamFormat
 * \param streamFormat
 */
void SensorControl::setStreamFormat(ReadingTypes streamFormat){

    //stop listener thread if it is still running
    if(this->listenerThread.isRunning()){
        this->listenerThread.quit();
        this->listenerThread.wait();
    }

    this->sensorListener->setReadingStreamFormat(streamFormat);

    //check sensor
    if(this->sensor.isNull() || !this->sensor->getConnectionState()){
        return;
    }

    //restart sensor listener
    this->sensorListener->moveToThread(&this->listenerThread);
    this->listenerThread.start();

}

/*!
 * \brief SensorControl::connectSensor
 */
void SensorControl::connectSensor(){

    this->locker.lock();

    //check sensor
    if(this->sensor.isNull()){
        locker.unlock();
        return;
    }

    //connect sensor if it is not connected yet
    bool success = false;
    QString msg;
    if(!this->sensor->getConnectionState()){
        success = this->sensor->connectSensor();
        if(success){
            msg = "sensor connected";
        }else{
            msg = "failed to connect sensor";
        }
    }else{
        msg = "sensor already connected";
    }

    emit this->commandFinished(success, msg);

    this->locker.unlock();

}

/*!
 * \brief SensorControl::disconnectSensor
 */
void SensorControl::disconnectSensor(){

    this->locker.lock();

    //check sensor
    if(this->sensor.isNull()){
        this->locker.unlock();
        return;
    }

    //disconnect sensor if it is connected
    bool success = false;
    QString msg;
    if(this->sensor->getConnectionState()){
        success = this->sensor->disconnectSensor();
        if(success){
            msg = "sensor disconnected";
        }else{
            msg = "failed to disconnect sensor";
        }
    }else{
        msg = "sensor not connected";
    }

    emit this->commandFinished(success, msg);

    this->locker.unlock();

}

/*!
 * \brief SensorControl::measure
 * \param geomId
 * \param mConfig
 */
void SensorControl::measure(const int &geomId, const MeasurementConfig &mConfig){

    this->locker.lock();

    //check sensor
    if(this->sensor.isNull()){
        this->locker.unlock();
        return;
    }

    //start measure if the sensor is connected
    bool success = false;
    QList<QPointer<Reading> > readings;
    QString msg;
    if(this->sensor->getConnectionState()){
        readings = this->sensor->measure(mConfig);
        if(readings.size() > 0){
            msg = "measurement finished";
            success = true;
        }else{
            msg = "failed to measure";
        }
    }else{
        msg = "sensor not connected";
    }

    emit this->commandFinished(success, msg);
    if(success){
        emit this->measurementFinished(geomId, readings);
    }

    this->locker.unlock();

}

/*!
 * \brief SensorControl::move
 * \param azimuth
 * \param zenith
 * \param distance
 * \param isRelative
 */
void SensorControl::move(const double &azimuth, const double &zenith, const double &distance, const bool &isRelative){

    this->locker.lock();

    //check sensor
    if(this->sensor.isNull()){
        this->locker.unlock();
        return;
    }

    //start moving if the sensor is connected
    bool success = false;
    QString msg;
    if(this->sensor->getConnectionState()){

        //set up sensor attributes
        SensorAttributes attr;
        attr.moveAzimuth = azimuth;
        attr.moveZenith = zenith;
        attr.moveDistance = distance;
        attr.moveIsRelative = isRelative;

        bool success = this->sensor->accept(eMoveAngle, attr);
        if(success){
            msg = "moving sensor finished";
            success = true;
        }else{
            msg = "failed to move sensor";
        }

    }else{
        msg = "sensor not connected";
    }

    emit this->commandFinished(success, msg);

    this->locker.unlock();

}

/*!
 * \brief SensorControl::move
 * \param x
 * \param y
 * \param z
 */
void SensorControl::move(const double &x, const double &y, const double &z){

    this->locker.lock();

    //check sensor
    if(this->sensor.isNull()){
        this->locker.unlock();
        return;
    }

    //start moving if the sensor is connected
    bool success = false;
    QString msg;
    if(this->sensor->getConnectionState()){

        //set up sensor attributes
        SensorAttributes attr;
        attr.moveX = x;
        attr.moveY = y;
        attr.moveZ = z;
        attr.moveIsRelative = false;

        bool success = this->sensor->accept(eMoveXYZ, attr);
        if(success){
            msg = "moving sensor finished";
            success = true;
        }else{
            msg = "failed to move sensor";
        }

    }else{
        msg = "sensor not connected";
    }

    emit this->commandFinished(success, msg);

    this->locker.unlock();

}

/*!
 * \brief SensorControl::initialize
 */
void SensorControl::initialize(){

    this->locker.lock();

    //check sensor
    if(this->sensor.isNull()){
        this->locker.unlock();
        return;
    }

    //start initializing if the sensor is connected
    bool success = false;
    QString msg;
    if(this->sensor->getConnectionState()){

        bool success = this->sensor->accept(eInitialize, SensorAttributes());
        if(success){
            msg = "initializing finished";
            success = true;
        }else{
            msg = "failed to initialize sensor";
        }

    }else{
        msg = "sensor not connected";
    }

    emit this->commandFinished(success, msg);

    this->locker.unlock();

}

/*!
 * \brief SensorControl::motorState
 */
void SensorControl::motorState(){

    this->locker.lock();

    //check sensor
    if(this->sensor.isNull()){
        this->locker.unlock();
        return;
    }

    //start changing motor state if the sensor is connected
    bool success = false;
    QString msg;
    if(this->sensor->getConnectionState()){

        bool success = this->sensor->accept(eMotorState, SensorAttributes());
        if(success){
            msg = "changing motor state finished";
            success = true;
        }else{
            msg = "failed to change motor state";
        }

    }else{
        msg = "sensor not connected";
    }

    emit this->commandFinished(success, msg);

    this->locker.unlock();

}

/*!
 * \brief SensorControl::home
 */
void SensorControl::home(){

    this->locker.lock();

    //check sensor
    if(this->sensor.isNull()){
        this->locker.unlock();
        return;
    }

    //start home if the sensor is connected
    bool success = false;
    QString msg;
    if(this->sensor->getConnectionState()){

        bool success = this->sensor->accept(eHome, SensorAttributes());
        if(success){
            msg = "setting home finished";
            success = true;
        }else{
            msg = "failed to set sensor to home position";
        }

    }else{
        msg = "sensor not connected";
    }

    emit this->commandFinished(success, msg);

    this->locker.unlock();

}

/*!
 * \brief SensorControl::toggleSight
 */
void SensorControl::toggleSight(){

    this->locker.lock();

    //check sensor
    if(this->sensor.isNull()){
        this->locker.unlock();
        return;
    }

    //start toggle sight if the sensor is connected
    bool success = false;
    QString msg;
    if(this->sensor->getConnectionState()){

        bool success = this->sensor->accept(eToggleSight, SensorAttributes());
        if(success){
            msg = "toggle sight orientation finished";
            success = true;
        }else{
            msg = "failed to toggle sight orientation";
        }

    }else{
        msg = "sensor not connected";
    }

    emit this->commandFinished(success, msg);

    this->locker.unlock();

}

/*!
 * \brief SensorControl::compensation
 */
void SensorControl::compensation(){

    this->locker.lock();

    //check sensor
    if(this->sensor.isNull()){
        this->locker.unlock();
        return;
    }

    //start compensation if the sensor is connected
    bool success = false;
    QString msg;
    if(this->sensor->getConnectionState()){

        bool success = this->sensor->accept(eCompensation, SensorAttributes());
        if(success){
            msg = "starting compensation finished";
            success = true;
        }else{
            msg = "failed to start compensation";
        }

    }else{
        msg = "sensor not connected";
    }

    emit this->commandFinished(success, msg);

    this->locker.unlock();

}

/*!
 * \brief SensorControl::selfDefinedAction
 * \param action
 */
void SensorControl::selfDefinedAction(const QString &action){

    this->locker.lock();

    //check sensor
    if(this->sensor.isNull()){
        this->locker.unlock();
        return;
    }

    //start compensation if the sensor is connected
    bool success = false;
    QString msg;
    if(this->sensor->getConnectionState()){

        bool success = this->sensor->doSelfDefinedAction(action);
        if(success){
            msg = "self defined action finished";
            success = true;
        }else{
            msg = "failed to do self defined action";
        }

    }else{
        msg = "sensor not connected";
    }

    emit this->commandFinished(success, msg);

    this->locker.unlock();

}
