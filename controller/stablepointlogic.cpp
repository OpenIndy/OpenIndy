#include "stablepointlogic.h"

using namespace oi;

StablePointLogic::StablePointLogic(MeasurementConfig config, QObject *parent) : QObject(parent),
    pointIsStable(false), config(config)
{

    static QTimer *timer = new QTimer(parent);
    connect(timer, SIGNAL(timeout()), this, SLOT(checkStablePoint()));
    timer->start(250);
}

StablePointLogic::StablePointLogic(const StablePointLogic &copy, QObject *parent) : QObject(parent), pointIsStable(copy.pointIsStable), config(copy.config){
}

StablePointLogic &StablePointLogic::operator=(const StablePointLogic &copy) {
    this->pointIsStable = copy.pointIsStable;
    this->config = copy.config;
    return *this;
}

/*!
 * \brief WatchWindowDialog::~WatchWindowDialog
 */
StablePointLogic::~StablePointLogic(){
    //
}

// signal  controller
void StablePointLogic::checkStablePoint() {
    if(this->pointIsStable) {
        this->pointIsStable = false;

        emit this->startMeasurement();
    }
}

/*!
 * \brief WatchWindowDialog::realTimeReading
 * \param reading
 */
void StablePointLogic::realTimeReading(const QVariantMap &reading){

    //check current job
    if(this->job.isNull()){
        return;
    }

    QPointer<Station> activeStation = this->job->getActiveStation();
    //check active station
    if(activeStation.isNull() || activeStation->getCoordinateSystem().isNull()){
        return;
    }

    if(true /*this->settings.readingType == eCartesianReading*/){
        // calculate stable

        if(!reading.contains("x") || !reading.contains("y") || !reading.contains("z")){
            return;
        }

        /*
         *  reading.value("x").toDouble();
         *  reading.value("y").toDouble();
         *  reading.value("z").toDouble();
        */
    }
}

void StablePointLogic::stopStablePointMeasurement(){
    QPointer<Station> activeStation = this->job->getActiveStation();

    //stop reading stream
    if(!activeStation.isNull() && activeStation->getIsSensorConnected()){
        emit this->stopStreaming();
    }
}

void StablePointLogic::startStablePointMeasurement(){

    //connect sensor
    this->connectSensor();

    //start reading stream
    emit this->startStreaming(eCartesianReading);
}

/*!
 * \brief WatchWindowDialog::connectSensor
 * Connects the active sensor so that real time readings are recognized by the watch window
 */
void StablePointLogic::connectSensor(){

    //check current job
    if(this->job.isNull()){
        return;
    }

    //get and check the active station
    QPointer<Station> activeStation = this->job->getActiveStation();
    if(activeStation.isNull()){
        return;
    }

    //check and disconnect the old station
    if(!activeStation.isNull()){
        QObject::disconnect(activeStation, &Station::realTimeReading, this, &StablePointLogic::realTimeReading);
    }

    //save and connect active station
    //this->activeStation = activeStation;
    QObject::connect(activeStation, &Station::realTimeReading, this, &StablePointLogic::realTimeReading);
}



