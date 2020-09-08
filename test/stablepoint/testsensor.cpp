#include "testsensor.h"

TestSensor::TestSensor(QObject *parent) : QObject(parent)
{

}
TestSensor::~TestSensor()
{

}

QVariantMap TestSensor::cartesianReading(double x, double y, double z) {
    QVariantMap reading;
    reading.insert("x", QString::number(x));
    reading.insert("y", QString::number(y));
    reading.insert("z", QString::number(z));
    return reading;
}

void TestSensor::process() {
    qDebug() << "process";

    int sleep = 200;

    // move phase, step 2 mm
    emit realTimeReading(cartesianReading(110.01, 200.01, 300.01));
    QThread::msleep(sleep);
    emit realTimeReading(cartesianReading(108.01, 200.01, 300.01));
    QThread::msleep(sleep);
    emit realTimeReading(cartesianReading(106.01, 200.01, 300.01));
    QThread::msleep(sleep);
    emit realTimeReading(cartesianReading(104.01, 200.01, 300.01));
    QThread::msleep(sleep);
    emit realTimeReading(cartesianReading(102.21, 200.01, 300.01));

    // stable phase
    QThread::msleep(sleep);
    emit realTimeReading(cartesianReading(100.12, 200.01, 300.01));
    QThread::msleep(sleep);
    emit realTimeReading(cartesianReading(100.13, 200.01, 300.01));
    QThread::msleep(sleep);
    emit realTimeReading(cartesianReading(100.01, 200.01, 300.01));
    QThread::msleep(sleep);
    emit realTimeReading(cartesianReading(100.03, 200.01, 300.01));
    QThread::msleep(sleep);
    emit realTimeReading(cartesianReading(100.04, 200.01, 300.01));
    QThread::msleep(sleep);
    emit realTimeReading(cartesianReading(100.05, 200.01, 300.01));
    QThread::msleep(sleep);
    emit realTimeReading(cartesianReading(100.01, 200.01, 300.01));
    QThread::msleep(sleep);
    emit realTimeReading(cartesianReading(100.03, 200.01, 300.01));
    QThread::msleep(sleep);
    emit realTimeReading(cartesianReading(100.04, 200.01, 300.01));

    /*
    // move phase, step 2 mm
    emit realTimeReading(cartesianReading(98.01, 200.01, 300.01));
    QThread::msleep(sleep);
    emit realTimeReading(cartesianReading(96.01, 200.01, 300.01));
    QThread::msleep(sleep);
    emit realTimeReading(cartesianReading(94.01, 200.01, 300.01));
    QThread::msleep(sleep);
    emit realTimeReading(cartesianReading(92.01, 200.01, 300.01));
    QThread::msleep(sleep);
    emit realTimeReading(cartesianReading(90.21, 200.01, 300.01));


    // stable phase
    QThread::msleep(sleep);
    emit realTimeReading(cartesianReading(88.12, 200.01, 300.01));
    QThread::msleep(sleep);
    emit realTimeReading(cartesianReading(88.13, 200.01, 300.01));
    QThread::msleep(sleep);
    emit realTimeReading(cartesianReading(88.01, 200.01, 300.01));
    QThread::msleep(sleep);
    emit realTimeReading(cartesianReading(88.03, 200.01, 300.01));
    QThread::msleep(sleep);
    emit realTimeReading(cartesianReading(88.04, 200.01, 300.01));
    QThread::msleep(sleep);
    emit realTimeReading(cartesianReading(88.05, 200.01, 300.01));
    QThread::msleep(sleep);
    emit realTimeReading(cartesianReading(88.01, 200.01, 300.01));
    QThread::msleep(sleep);
    emit realTimeReading(cartesianReading(88.03, 200.01, 300.01));
    QThread::msleep(sleep);
    emit realTimeReading(cartesianReading(88.04, 200.01, 300.01));
*/
    emit finished();
}
