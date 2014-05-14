#include "sensorlistener.h"

SensorListener::SensorListener(Sensor* s)
{
    instrument = s;
    isStreamActive = false;
    isStreamFinished = false;
}


void SensorListener::sensorStatStream()
{
    isStreamFinished = false;

    while(isStreamActive){
        QMap<QString,QString> m= instrument->getSensorStats();
        emit sendSensorStats(m);
    }

    isStreamFinished = true;
}

void SensorListener::sensorReadingStream(Configuration::ReadingTypes streamFormat)
{
    isStreamFinished = false;

    while(isStreamActive){
        QVariantMap m = instrument->readingStream(streamFormat);
        emit sendReadingMap(m);
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






