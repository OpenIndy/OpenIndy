#ifndef OISENSOREMITTER_H
#define OISENSOREMITTER_H

#include "oiemitter.h"

class OiSensorEmitter : public OiEmitter
{

    Q_OBJECT

public:

    static const int sensorOk; //1
    static const int sensorWarning; //2
    static const int sensorError; //0

    explicit OiSensorEmitter(QObject *parent = 0);

signals:
    void sendConnectionStat(bool);
    void sendIsReadyForMeasurement(int sensorState, QString stateMsg);
    void sendSensorError(QString);
    void sendMeasureTrigger();

public slots:
    void emitSendConnectionStat(bool);
    void emitSendIsReadyForMeasurement(int sensorState, QString stateMsg);
    void emitSendSensorError(QString);
    void emitSendMeasureTrigger();

};

#endif // OISENSOREMITTER_H
