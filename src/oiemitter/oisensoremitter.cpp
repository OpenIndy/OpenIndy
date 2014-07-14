#include "oisensoremitter.h"

OiSensorEmitter::OiSensorEmitter(QObject *parent) :
    OiEmitter(parent)
{
}


void OiSensorEmitter::emitSendConnectionStat(bool b)
{
    emit sendConnectionStat(b);
}

void OiSensorEmitter::emitSendIsReadyForMeasurement(bool b)
{
    emit sendIsReadyForMeasurement(b);
}

void OiSensorEmitter::emitSendSensorError(QString s)
{
    emit sendSensorError(s);
}

void OiSensorEmitter::emitSendMeasureTrigger()
{
    emit sendMeasureTrigger();
}
