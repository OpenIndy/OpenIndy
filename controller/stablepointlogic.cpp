#include "stablepointlogic.h"

#define DEBUG_READINGDATA(rd) "elapsed" << rd.elapsed \
    << "xyz" << rd.xyz.getAt(0) <<  rd.xyz.getAt(1) <<  rd.xyz.getAt(2) \
    << "guessStable" <<  rd.guessStable \
    << "distanceToPrevReading" << (rd.distanceToPrevReading < DBL_MAX ? QString::number(rd.distanceToPrevReading, 'f', 5) : "max" )  << "[m]" \
    << "distanceToPrevStable" << (rd.distanceToPrevStable < DBL_MAX ? QString::number(rd.distanceToPrevStable, 'f', 5) : "max" ) << "[m]"

using namespace oi;
using namespace oi::math;

StablePointLogic::StablePointLogic(QObject *parent) : QObject(parent), pointIsStable(false) {
    this->init();
}

StablePointLogic::StablePointLogic(const StablePointLogic &copy, QObject *parent) : QObject(parent), pointIsStable(false) {
    this->init();
}

StablePointLogic &StablePointLogic::operator=(const StablePointLogic &copy) {
    this->pointIsStable = false;
    this->config = copy.config;
    return *this;
}

void StablePointLogic::init() {
    checkStableTimer = new QTimer(this);
    connect(checkStableTimer, SIGNAL(timeout()), this, SLOT(checkStablePoint()));
    checkStableTimer->start(250);
}

/*!
 * \brief WatchWindowDialog::~WatchWindowDialog
 */
StablePointLogic::~StablePointLogic(){
    //
}

// signal controller
void StablePointLogic::checkStablePoint() {
    qDebug() << "checkStablePoint: pointIsStable" << this->pointIsStable;

    if(this->pointIsStable) {
        this->pointIsStable = false;
        emit this->stopStreaming();
        emit this->startMeasurement();
        emit this->startStreaming(ReadingTypes::eCartesianReading);
    }
}

void StablePointLogic::euclideanDistance(double &result, OiVec v1, OiVec v2) {
    double dot;
    OiVec v = v2 - v1;
    OiVec::dot(dot, v, v);
    result = qSqrt(dot);
}

/*!
 * \brief WatchWindowDialog::realTimeReading
 * \param reading
 */
void StablePointLogic::realTimeReading(const QVariantMap &reading){
    // recieve readings
    if(!reading.contains("x") || !reading.contains("y") || !reading.contains("z")){
        return;
    }

    OiVec xyz(3);
    xyz.setAt(0, reading.value("x").toDouble());
    xyz.setAt(1, reading.value("y").toDouble());
    xyz.setAt(2, reading.value("z").toDouble());

    qDebug() << "size 1" << readingDatas.size();
    if(readingDatas.size() > 0) { // 2. to n. call

        // distance to prevoius reading
        double distance;
        euclideanDistance(distance, xyz, readingDatas.last().xyz);

        // distance to last stable point
        double lastMeasuredPointDistance  = DBL_MAX;
        if(lastStableXyz.getSize() > 0) {
            euclideanDistance(lastMeasuredPointDistance, xyz, lastStableXyz);
        }

        // create current ReadingData and add to queue
        ReadingData rd;
        rd.elapsed = elapsedTimer.elapsed();
        rd.xyz = xyz;
        rd.distanceToPrevReading = distance;
        rd.distanceToPrevStable = lastMeasuredPointDistance;
        // guess stable because distance is ok
        rd.guessStable = distance * 1000. < config.getStablePointThresholdRange() && lastMeasuredPointDistance * 1000. > config.getStablePointMinDistance();
        qDebug() << DEBUG_READINGDATA(rd);

        readingDatas.enqueue(rd);

        // removing old ReadingData by time
        while(rd.elapsed - readingDatas.head().elapsed > this->config.getStablePointThresholdTime() * 1000.) {
            qDebug() << "remove " << DEBUG_READINGDATA(readingDatas.head());
            readingDatas.dequeue();
        }

        qDebug() << "size 2" << readingDatas.size();
        // queue contains ReadingData in time range only
        if(readingDatas.size() > 0) { // min readings in time range
            // checking if all ReadingData in time range are "guessStable"
            this->pointIsStable = true;
            for (int i = 0; i < readingDatas.size(); ++i) {
                qDebug() << "    " << i << DEBUG_READINGDATA(readingDatas.at(i));
                if(! readingDatas.at(i).guessStable) {
                    this->pointIsStable = false;
                    break;
                }
            }

            // checking if stable point is found
            if(this->pointIsStable) {
                this->lastStableXyz.replace(xyz);
                qDebug() << "stable" << reading;
                qDebug() << "lastStableXyz"
                             << lastStableXyz.getAt(0)
                             << lastStableXyz.getAt(1)
                             << lastStableXyz.getAt(2);
            }
        }

        qDebug() << "pointIsStable" << this->pointIsStable;


    } else { // 1. call
        ReadingData rd;
        rd.elapsed = elapsedTimer.elapsed();
        rd.xyz = xyz;
        rd.guessStable = false;
        rd.distanceToPrevReading = DBL_MAX;
        rd.distanceToPrevStable = DBL_MAX;
        qDebug() << DEBUG_READINGDATA(rd);
        readingDatas.enqueue(rd);
    }

}

void StablePointLogic::stopStablePointMeasurement(){
    emit this->stopStreaming();
    if(!checkStableTimer.isNull()) {
        checkStableTimer->stop();
    }

}

void StablePointLogic::startStablePointMeasurement(MeasurementConfig config){
    this->config = config;

    qDebug() << "startStablePointMeasurement";

    // keep: this->lastStableXyz
    this->readingDatas.clear(); // remove previouse
    this->elapsedTimer.start();

    emit this->startStreaming(ReadingTypes::eCartesianReading);
}
