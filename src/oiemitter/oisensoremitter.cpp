#include "oisensoremitter.h"

OiSensorEmitter::OiSensorEmitter(QObject *parent) :
    OiEmitter(parent)
{
}


void OiSensorEmitter::emitSendConnectionStat(bool b)
{
    emit sendConnectionStat(b);
}

void OiSensorEmitter::emitSendIsReadyForMeasurement(OiSensorEmitter::sensorState s)
{
    emit sendIsReadyForMeasurement(s);
}

void OiSensorEmitter::emitSendSensorError(QString s)
{
    emit sendSensorError(s);
}

void OiSensorEmitter::emitSendMeasureTrigger()
{
    emit sendMeasureTrigger();
}
