#include "sensorlistener.h"

SensorListener::SensorListener(Sensor* s)
{
    instrument = s;
    isCheckStreamActive = false;
    checkStreamFinished = false;
}

void SensorListener::sensorCheckStream()
{


    checkStreamFinished = false;

    while(isCheckStreamActive){
        bool conCheck = instrument->getConnectionState();
        emit sendConnectionState(conCheck);
        bool readyCheck = instrument->isReady();
        emit sendIsReady(readyCheck);
    }

    checkStreamFinished = true;

}

void SensorListener::abortSensorAction()
{
    instrument->abortAction();
}


void SensorListener::emitReadingMap(QVariantMap m)
{

    emit sendReadingMap(m);
}

void SensorListener::emitSensorStats(QMap<QString, QString> m)
{
    emit sendSensorStats(m);
}

void SensorListener::setInstrument(Sensor *s)
{
    instrument = s;
}

Sensor *SensorListener::getInstrument()
{
    return instrument;
}



