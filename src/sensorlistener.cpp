#include "sensorlistener.h"

/*!
 * \brief SensorListener::SensorListener
 * \param locker
 * \param parent
 */
SensorListener::SensorListener(QMutex &locker, QObject *parent) : QObject(parent),
    locker(locker), streamFormat(eCartesianReading){

}

/*!
 * \brief SensorListener::~SensorListener
 */
SensorListener::~SensorListener(){

}

/*!
 * \brief SensorListener::setInstrument
 * \param sensor
 */
void SensorListener::setSensor(const QPointer<Sensor> &sensor){
    if(!sensor.isNull()){
        this->sensor = sensor;
    }
}

/*!
 * \brief SensorListener::getInstrument
 * \return
 */
const QPointer<Sensor> &SensorListener::getSensor() const{
    return this->sensor;
}

/*!
 * \brief SensorListener::startStreaming
 */
void SensorListener::startStreaming(){

    //get real time information from sensor as long the sensor is connected
    forever{

        this->locker.lock();

        //check sensor
        if(this->sensor.isNull()){
            emit this->connectionLost();
            this->locker.unlock();
            return;
        }

        //if the tracker is busy do not call tracker methods
        if(!this->sensor->getIsBusy()){

            //check connection state
            if(!this->sensor->getConnectionState()){
                emit this->connectionLost();
                this->locker.unlock();
                QThread::msleep(100);
                continue;
            }

            //get sensor status information
            //QMap<QString,QString> status = this->sensor->getSensorStatus();
            //emit this->realTimeStatus(status);

            //get reading (current sensor position)
            QVariantMap reading = this->sensor->readingStream(this->streamFormat);
            emit this->realTimeReading(reading);

        }

        this->locker.unlock();

        //sleep for 20 ms to avoid overhead
        QThread::msleep(20);

    }

}

/*!
 * \brief SensorListener::getReadingStreamFormat
 * \return
 */
ReadingTypes SensorListener::getReadingStreamFormat() const{
    return this->streamFormat;
}

/*!
 * \brief SensorListener::setReadingStreamFormat
 * \param streamFormat
 */
void SensorListener::setReadingStreamFormat(ReadingTypes streamFormat){
    this->streamFormat = streamFormat;
}
