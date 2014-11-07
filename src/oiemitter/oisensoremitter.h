#ifndef OISENSOREMITTER_H
#define OISENSOREMITTER_H

#include "oiemitter.h"

class OiSensorEmitter : public OiEmitter
{

    Q_OBJECT

public:

    enum sensorState{
        sensorOk,
        sensorWarning,
        sensorError
    };

    explicit OiSensorEmitter(QObject *parent = 0);

signals:
    void sendConnectionStat(bool);
    void sendIsReadyForMeasurement(OiSensorEmitter::sensorState s);
    void sendSensorError(QString);
    void sendMeasureTrigger();

public slots:
    void emitSendConnectionStat(bool);
    void emitSendIsReadyForMeasurement(OiSensorEmitter::sensorState s);
    void emitSendSensorError(QString);
    void emitSendMeasureTrigger();

};

#endif // OISENSOREMITTER_H
