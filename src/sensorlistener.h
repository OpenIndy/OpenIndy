#ifndef SENSORLISTENER_H
#define SENSORLISTENER_H

#include <QObject>
#include <QThread>
#include <QEventLoop>
#include "sensor.h"

class SensorListener : public QObject
{
    Q_OBJECT
public:
    SensorListener(Sensor* s);

    bool isCheckStreamActive;
    bool checkStreamFinished;
    
signals:

    void sendConnectionState(bool);
    void sendIsReady(bool);

    void sendReadingMap(QVariantMap);
    void sendSensorStats(QMap<QString,QString>);
    
public slots:

    void sensorCheckStream();
    void abortSensorAction();

    void emitReadingMap(QVariantMap m);
    void emitSensorStats(QMap<QString,QString>);

    void setInstrument(Sensor* s);
    Sensor* getInstrument();

private:
    Sensor* instrument;

};

#endif // SENSORLISTENER_H
