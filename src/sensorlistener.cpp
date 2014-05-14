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

    if(instrument != NULL){
        while(isStreamActive){
            QMap<QString,QString> m= instrument->getSensorStats();
            emit sendSensorStats(m);
        }
    }

    isStreamFinished = true;
}

void SensorListener::sensorReadingStream(int streamFormat)
{
    isStreamFinished = false;

    Configuration::ReadingTypes readingTyp = (Configuration::ReadingTypes) streamFormat;

    if(instrument != NULL){
        while(isStreamActive){
            QVariantMap m = instrument->readingStream(readingTyp);
            emit sendReadingMap(m);
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






