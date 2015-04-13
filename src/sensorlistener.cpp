#include "sensorlistener.h"
/*
SensorListener::SensorListener(Sensor* s)
{
    instrument = s;
    isStreamActive = false;
    isStreamFinished = false;
}


void SensorListener::sensorStatStream()
{
    isStreamFinished = false;

    if(instrument != NULL){
        while(isStreamActive){
            if(instrument->getConnectionState()){
                QMap<QString,QString> m= instrument->getSensorStats();
                emit sendSensorStats(m);
            }else{
                emit connectionLost();
            }
        }
    }

    isStreamFinished = true;
}

void SensorListener::sensorReadingStream(int streamFormat)
{
    isStreamFinished = false;

    ReadingTypes readingTyp = (ReadingTypes) streamFormat;

    if(instrument != NULL && instrument->getConnectionState()){
        while(isStreamActive){
            if(instrument->getConnectionState()){
               QVariantMap m = instrument->readingStream(readingTyp);
               emit sendReadingMap(m);
            }else{
               emit connectionLost();
            }
        }
    }

    isStreamFinished = true;
}

void SensorListener::abortSensorAction()
{
    instrument->abortAction();
}

void SensorListener::setInstrument(Sensor *s)
{
    instrument = s;
}

Sensor *SensorListener::getInstrument()
{
    return instrument;
}
*/


/*!
 * \brief SensorListener::SensorListener
 * \param sensor
 * \param parent
 */
SensorListener::SensorListener(const QPointer<Sensor> &sensor, QObject *parent) : QObject(parent){

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
void SensorListener::setInstrument(QPointer<Sensor> sensor){
    if(!sensor.isNull()){
        this->sensor = sensor;
    }
}

/*!
 * \brief SensorListener::getInstrument
 * \return
 */
QPointer<Sensor> SensorListener::getInstrument() const{
    return this->sensor;
}

/*!
 * \brief SensorListener::getIsStreamActive
 * \return
 */
bool SensorListener::getIsStreamActive() const{
    return this->isStreamActive;
}

/*!
 * \brief SensorListener::startStatusStream
 */
void SensorListener::startStatusStream(){

}

/*!
 * \brief SensorListener::startReadingStream
 * \param streamFormat
 */
void SensorListener::startReadingStream(const ReadingTypes &streamFormat){

}

/*!
 * \brief SensorListener::abortSensorAction
 */
void SensorListener::abortSensorAction(){

}
