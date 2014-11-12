#include "oisensoremitter.h"

const int OiSensorEmitter::sensorOk = 1;
const int OiSensorEmitter::sensorWarning = 2;
const int OiSensorEmitter::sensorError = 0;

OiSensorEmitter::OiSensorEmitter(QObject *parent) :
    OiEmitter(parent)
{
}


void OiSensorEmitter::emitSendConnectionStat(bool b)
{
    emit sendConnectionStat(b);
}

void OiSensorEmitter::emitSendIsReadyForMeasurement(int sensorState, QString stateMsg)
{
    emit sendIsReadyForMeasurement(sensorState,stateMsg);
}

void OiSensorEmitter::emitSendSensorError(QString s)
{
    emit sendSensorError(s);
}

void OiSensorEmitter::emitSendMeasureTrigger()
{
    emit sendMeasureTrigger();
}
