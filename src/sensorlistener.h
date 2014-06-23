#ifndef SENSORLISTENER_H
#define SENSORLISTENER_H

#include <QObject>
#include <QThread>
#include <QTime>
#include "sensor.h"

/*!
 * \brief The SensorListener class
 * the sensor listener handles all sensor streams
 * it should be run on a custom thread and the
 * sensorcontroller of the station delegates the
 * stream activity
 */
class SensorListener : public QObject
{
    Q_OBJECT
public:
    SensorListener(Sensor* s);


    /*booleans to check and control
     *stream activity
    */
    bool isStreamActive;
    bool isStreamFinished;
    
signals:

    void sendReadingMap(QVariantMap);
    void sendSensorStats(QMap<QString,QString>);
    void connectionLost();
    
public slots:

    //streams
    void sensorStatStream();
    void sensorReadingStream(int streamFormat);

    //abort actual sensor action
    void abortSensorAction();

    //sensor
    void setInstrument(Sensor* s);
    Sensor* getInstrument();

private:
    Sensor* instrument;

};

#endif // SENSORLISTENER_H
