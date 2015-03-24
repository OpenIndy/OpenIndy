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






