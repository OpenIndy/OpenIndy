#include "mocksensor.h"

MockSensor::MockSensor(int sleep, QString readings, QObject *parent) : QThread(parent), sleep(sleep), readings(readings)
{
    streaming = false;
}
MockSensor::~MockSensor()
{

}

void MockSensor::run() {
    qDebug() << "MockSensor::run";

    QQueue<QString> readingQ;
    readingQ.append(readings.split(QRegExp("[\r\n]"), QString::SkipEmptyParts));

    while(!readingQ.isEmpty()) {
        if(streaming) {
            QString r = readingQ.dequeue();
            if(r.startsWith("#")) {
                continue;
            }

            QStringList xzy = r.split(QRegExp("[ ]"), QString::SkipEmptyParts);
            QVariantMap readingMap;
            readingMap.insert("x", xzy.at(0));
            readingMap.insert("y", xzy.at(1));
            readingMap.insert("z", xzy.at(2));

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
