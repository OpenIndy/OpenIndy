#ifndef OIEMITTER_H
#define OIEMITTER_H

#include <QObject>
#include <QVariant>
#include <QMap>
#include "reading.h"


class OiEmitter : public QObject
{
    Q_OBJECT
public:
    explicit OiEmitter(QObject *parent = 0);

signals:
    void sendString(QString);
    void sendConnectionStat(bool);
    void sendIsReadyForMeasurement(bool);
    void sendSensorError(QString);
    void sendMeasureTrigger();

public slots:
    void emitSendString(QString);
    void emitSendConnectionStat(bool);
    void emitSendIsReadyForMeasurement(bool);
    void emitSendSensorError(QString);
    void emitSendMeasureTrigger();

};

#endif // OIEMITTER_H
