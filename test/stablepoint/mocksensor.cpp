#include "mocksensor.h"

MockSensor::MockSensor(int sleep, QString readings, QObject *parent) : QThread(parent), sleep(sleep), readings(readings)
{
    streaming = false;

    readingQ.append(readings.split(QRegExp("[\r\n]"), QString::SkipEmptyParts));
}
MockSensor::~MockSensor()
{

}

void MockSensor::run() {
    qDebug() << "MockSensor::run";

    QElapsedTimer elapsedTimer;
    elapsedTimer.start();
    while(!readingQ.isEmpty()) {
        if(streaming) {
            QString r = readingQ.dequeue();
            if(r.startsWith("#")) {
                qDebug() << "MockSensor" << r;
                continue; // no sleep
            }

            QStringList xyz = r.split(QRegExp("[ ]"), QString::SkipEmptyParts);
            QVariantMap readingMap;
            readingMap.insert("x", xyz.at(0));
            readingMap.insert("y", xyz.at(1));
            readingMap.insert("z", xyz.at(2));
            readingMap.insert("elapsed", elapsedTimer.elapsed());

            qDebug() << "MockSensor" << readingMap;
            emit realTimeReading(readingMap);
        }

        QThread::msleep(sleep);

    }

}

void MockSensor::stopStreaming() {
    qDebug() << "MockSensor::stopStreaming";
    this->streaming = false;
}

void MockSensor::startStreaming(ReadingTypes rt) {
    qDebug() << "MockSensor::startStreaming";
    this->streaming = true;
}
