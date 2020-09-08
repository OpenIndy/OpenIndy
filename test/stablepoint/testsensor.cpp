#include "testsensor.h"

TestSensor::TestSensor(int sleep, QString readings, QObject *parent) : QThread(parent), sleep(sleep), readings(readings)
{
    streaming = false;
}
TestSensor::~TestSensor()
{

}

void TestSensor::run() {
    qDebug() << "run";

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
            readingMap.insert("z", xzy.at(0));

            emit realTimeReading(readingMap);
        }

        QThread::msleep(sleep);

    }

}

void TestSensor::stopStreaming() {
    qDebug() << "stopStreaming";
    this->streaming = false;
}

void TestSensor::startStreaming(ReadingTypes rt) {
    qDebug() << "startStreaming";
    this->streaming = true;
}
