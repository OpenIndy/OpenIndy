#ifndef OISENSOREMITTER_H
#define OISENSOREMITTER_H

#include "oiemitter.h"

class OiSensorEmitter : public OiEmitter
{

    Q_OBJECT

public:
    explicit OiSensorEmitter(QObject *parent = 0);

signals:
    void sendConnectionStat(bool);
    void sendIsReadyForMeasurement(bool);
    void sendSensorError(QString);
    void sendMeasureTrigger();

public slots:
    void emitSendConnectionStat(bool);
    void emitSendIsReadyForMeasurement(bool);
    void emitSendSensorError(QString);
    void emitSendMeasureTrigger();

};

#endif // OISENSOREMITTER_H
