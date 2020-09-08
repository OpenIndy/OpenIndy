#include "stablepointlogic.h"

using namespace oi;

StablePointLogic::StablePointLogic(MeasurementConfig config, QObject *parent) : QObject(parent),
    pointIsStable(false), config(config)
{

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
    emit this->stopStreaming();

}

void StablePointLogic::startStablePointMeasurement(){
    qDebug() << "start timer";
    QPointer<QTimer> timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(checkStablePoint()));
    timer->start(250);

    emit this->startStreaming(ReadingTypes::eCartesianReading);
}
