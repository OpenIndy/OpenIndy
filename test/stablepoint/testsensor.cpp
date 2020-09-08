#include "testsensor.h"

TestSensor::TestSensor(int sleep, QString readings, QObject *parent) : QObject(parent), sleep(sleep), readings(readings)
{

}
TestSensor::~TestSensor()
{

}

void TestSensor::process() {
    qDebug() << "process";

    QStringList readingL = readings.split(QRegExp("[\r\n]"), QString::SkipEmptyParts);
    for ( const QString& r : readingL ) {
        if(r.contains("#")) {
            continue;
        }

        QStringList xzy = r.split(QRegExp("[ ]"), QString::SkipEmptyParts);
        QVariantMap readingMap;
        readingMap.insert("x", xzy.at(0));
        readingMap.insert("y", xzy.at(1));
        readingMap.insert("z", xzy.at(0));

        emit realTimeReading(readingMap);

        QThread::msleep(sleep);

    }

    emit finished();
}
