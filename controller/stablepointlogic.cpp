#include "stablepointlogic.h"

using namespace oi;
using namespace oi::math;

StablePointLogic::StablePointLogic(MeasurementConfig config, QObject *parent) : QObject(parent),
    pointIsStable(false), config(config)
{

}

StablePointLogic::StablePointLogic(const StablePointLogic &copy, QObject *parent) : QObject(parent), pointIsStable(false), config(config) {
}

StablePointLogic &StablePointLogic::operator=(const StablePointLogic &copy) {
    this->pointIsStable = false;
    this->config = copy.config;
    return *this;
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

        emit this->startMeasurement();
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

    if(!reading.contains("x") || !reading.contains("y") || !reading.contains("z")){
        return;
    }

    OiVec xyz(3);
    xyz.setAt(0, reading.value("x").toDouble());
    xyz.setAt(1, reading.value("y").toDouble());
    xyz.setAt(2, reading.value("z").toDouble());

    if(readingDatas.size() > 0) { // 2-n call
        double distance;
        euclideanDistance(distance, xyz, readingDatas.last().xyz);

        double lastMeasuredPointDistance  = DBL_MAX;
        if(lastStableXyz.getSize() > 0) {
            euclideanDistance(lastMeasuredPointDistance, xyz, lastStableXyz);
        }

        ReadingData rd;
        rd.elapsed = elapsedTimer.elapsed();
        rd.xyz = xyz;
        rd.distanceToPrevReading = distance;
        rd.distanceToPrevStable = lastMeasuredPointDistance;
        rd.guessStable = distance < config.getStablePointThresholdRange() && lastMeasuredPointDistance > config.getStablePointMinDistance();
        qDebug() << "elapsed" << rd.elapsed
                 << "xyz" << reading // rd.xyz
                 << "guessStable" << rd.guessStable
                 << "distanceToPrevReading" << rd.distanceToPrevReading
                 << "distanceToPrevStable" << rd.distanceToPrevStable;
        readingDatas.enqueue(rd);

        // remove old ReadingData
        while(rd.elapsed - readingDatas.head().elapsed > this->config.getStablePointThresholdTime() * 1000) {
            readingDatas.dequeue();
        }

        if(readingDatas.size() > 3) { // min readings in time range
            // check if all true
            this->pointIsStable = true;
            for (int i = 0; i < readingDatas.size(); ++i) {
                qDebug() << "    " << i <<  readingDatas.at(i).guessStable;
                if(! readingDatas.at(i).guessStable) {
                    this->pointIsStable = false;
                    break;
                }
            }
        }

        qDebug() << "pointIsStable" << this->pointIsStable;


    } else {
        ReadingData rd;
        rd.elapsed = elapsedTimer.elapsed();
        rd.xyz = xyz;
        rd.guessStable = false;
        rd.distanceToPrevReading = DBL_MAX;
        rd.distanceToPrevStable = DBL_MAX;
        qDebug() << "elapsed" << rd.elapsed
                 << "xyz" << reading // rd.xyz
                 << "guessStable" << rd.guessStable
                 << "distanceToPrevReading" << rd.distanceToPrevReading
                 << "distanceToPrevStable" << rd.distanceToPrevStable;
        readingDatas.enqueue(rd);
    }

}

void StablePointLogic::stopStablePointMeasurement(){
    emit this->stopStreaming();

}

void StablePointLogic::startStablePointMeasurement(){
    qDebug() << "startStablePointMeasurement";
    QPointer<QTimer> timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(checkStablePoint()));
    timer->start(250);

    this->elapsedTimer.start();

    emit this->startStreaming(ReadingTypes::eCartesianReading);
}
