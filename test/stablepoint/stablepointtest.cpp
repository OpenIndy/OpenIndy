#include <QString>
#include <QtTest>

#include <QPointer>

#include "position.h"
#include "oivec.h"
#include "chooselalib.h"
#include "util.h"
#include "stablepointlogic.h"
#include "testsensor.h"

#define COMPARE_DOUBLE(actual, expected, threshold) QVERIFY(std::abs(actual-expected)< threshold);
#define _OI_VEC(v) v.getAt(0) << "," << v.getAt(1) << "," << v.getAt(2)
#define DEBUG_RESULT(result)  qDebug() << qSetRealNumberPrecision(10) << "\nx, y, z, dx, dy, dz, d3D, radius\n" << _OI_VEC(result.position.getVector()) << "," << _OI_VEC(result.delta) << "," << result.d3D << "," << result.radius.getRadius();

using namespace oi;
using namespace oi::math;


class StablePointTest : public QObject
{
    Q_OBJECT

public:
    StablePointTest();

private Q_SLOTS:
    void testStablePoint1();


private:
    OiVec createTrackerXYZ(double x, double y, double z);
};

StablePointTest::StablePointTest()
{
}

OiVec StablePointTest::createTrackerXYZ(double x, double y, double z) {
    OiVec trackerXYZ = OiVec(4);
    trackerXYZ.setAt(0, x);
    trackerXYZ.setAt(1, y);
    trackerXYZ.setAt(2, z);
    trackerXYZ.setAt(3, 1.0);
    return trackerXYZ;
}

// basic test
void StablePointTest::testStablePoint1()
{

    ChooseLALib::setLinearAlgebra(ChooseLALib::Armadillo);

    //StablePointLogic *logic = new StablePointLogic(job, this);
    MeasurementConfig config;
    config.setIsStablePoint(true);
    config.setStablePointThresholdTime(1.); // [second]
    config.setStablePointMinDistance(1.0);  // [mm]

    QPointer<StablePointLogic> logic = new StablePointLogic(config, this);
    logic->startStablePointMeasurement();

    // send readings from sensor
    QPointer<TestSensor> sensor = new TestSensor();
    connect(sensor, &TestSensor::realTimeReading, logic, &StablePointLogic::realTimeReading, Qt::AutoConnection);
    QThread* sensorThread = new QThread();
    sensor->moveToThread(sensorThread);
    connect(sensorThread, SIGNAL (started()), sensor, SLOT (process()));
    sensorThread->start();


    QThread::msleep(1000);
    QSignalSpy spy_startMeasurement(logic, SIGNAL(startStreaming()));

    // check all 250ms first run is false
    QCOMPARE(spy_startMeasurement.wait(300), false);
    QCOMPARE(spy_startMeasurement.count(), 0);

    // start   
    QCOMPARE(spy_startMeasurement.wait(300), true);
    QCOMPARE(spy_startMeasurement.count(), 1);


    logic->stopStablePointMeasurement();

    QTEST_ASSERT(false);

}


QTEST_MAIN(StablePointTest) // instead of QTEST_APPLESS_MAIN because of QTimer

#include "stablepointtest.moc"
