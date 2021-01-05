#include <QString>
#include <QtTest>

#include <QPointer>

#include "position.h"
#include "oivec.h"
#include "chooselalib.h"
#include "util.h"
#include "stablepointlogic.h"
#include "mocksensor.h"

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
    void testStablePoint_basic();
    void testStablePoint_move_stable_move_stable();

};

StablePointTest::StablePointTest()
{
    qRegisterMetaType<oi::ReadingTypes>("ReadingTypes");
}

// basic test
void StablePointTest::testStablePoint_basic()
{

    ChooseLALib::setLinearAlgebra(ChooseLALib::Armadillo);

    // readings from sensor
    QString readings("\
# move phase, step 2 mm \n\
0.11001 0.20001 0.30001\n\
0.10801 0.20001 0.30001\n\
0.10601 0.20001 0.300.01\n\
0.10401 0.20001 0.30001\n\
0.10221 0.20001 0.30001\n\
# stable phase \n\
0.10012 0.20001 0.30001\n\
0.10013 0.20001 0.30001\n\
0.10001 0.20001 0.30001\n\
0.10003 0.20001 0.30001\n\
0.10004 0.20001 0.30001\n\
# stable 1 ...\n\
0.10002 0.20001 0.30001\n\
0.10003 0.20001 0.30001\n\
0.10001 0.20001 0.30001\n\
0.10003 0.20001 0.30001\n\
0.10004 0.20001 0.30001\n\
");


    MeasurementConfig config;
    config.setIsStablePoint(true);
    config.setStablePointThresholdTime(1.); // [second]
    config.setStablePointMinDistance(1.0);  // [mm]
    config.setStablePointThresholdRange(0.1); // [mm]

    QPointer<StablePointLogic> logic = new StablePointLogic(this);

    QPointer<MockSensor> sensor = new MockSensor(200, readings);
    sensor->start(); // starting background thread

    // connect sensor and logic
    connect(sensor, &MockSensor::realTimeReading, logic, &StablePointLogic::realTimeReading, Qt::AutoConnection);
    connect(logic, &StablePointLogic::stopStreaming, sensor, &MockSensor::stopStreaming);
    connect(logic, &StablePointLogic::startStreaming, sensor, &MockSensor::startStreaming);

    // start stable point measurement
    logic->startStablePointMeasurement(config);

    // check if "real" measurement is triggert
    QSignalSpy spy_startMeasurement(logic, SIGNAL(startMeasurement()));

    QCOMPARE(spy_startMeasurement.wait(3000), true);
    QCOMPARE(spy_startMeasurement.count(), 1);


    logic->stopStablePointMeasurement();
    sensor->terminate();
    sensor->wait(1000);

}

void StablePointTest::testStablePoint_move_stable_move_stable()
{

    ChooseLALib::setLinearAlgebra(ChooseLALib::Armadillo);

    // readings from sensor
    QString readings("\
# 1 second: move phase, step 2 mm\n\
0.11001 0.20001 0.30001\n\
0.10801 0.20001 0.30001\n\
0.10601 0.20001 0.30001\n\
0.10401 0.20001 0.30001\n\
0.10221 0.20001 0.30001\n\
# 3 seconds: stable phase\n\
0.10012 0.20001 0.30001\n\
0.10013 0.20001 0.30001\n\
0.10001 0.20001 0.30001\n\
0.10003 0.20001 0.30001\n\
0.10004 0.20001 0.30001\n\
##\n\
0.10005 0.20001 0.30001\n\
0.10001 0.20001 0.30001\n\
0.10003 0.20001 0.30001\n\
0.10004 0.20001 0.30001\n\
0.10001 0.20001 0.30001\n\
##\n\
0.10003 0.20001 0.30001\n\
0.10004 0.20001 0.30001\n\
0.10005 0.20001 0.30001\n\
0.10001 0.20001 0.30001\n\
0.10003 0.20001 0.30001\n\
# move phase, step 2 mm\n\
0.10001 0.20191 0.30001\n\
0.10001 0.20201 0.30001\n\
0.10001 0.20401 0.30001\n\
0.10001 0.20601 0.30001\n\
0.10001 0.20801 0.30001\n\
# stable phase\n\
0.10035 0.21001 0.30001\n\
0.10031 0.21001 0.30001\n\
0.10032 0.21001 0.30001\n\
0.10033 0.21001 0.30001\n\
0.10034 0.21001 0.30001\n\
##\n\
0.10035 0.21001 0.30001\n\
0.10031 0.21001 0.30001\n\
0.10031 0.21001 0.30001\n\
0.10032 0.21001 0.30001");

    MeasurementConfig config;
    config.setIsStablePoint(true);
    config.setStablePointThresholdTime(1.0); // [second]
    config.setStablePointThresholdRange(1.0); // [mm]
    config.setStablePointMinDistance(1.0);  // [mm]

    QPointer<StablePointLogic> logic = new StablePointLogic(this);

    QPointer<MockSensor> sensor = new MockSensor(200, readings);
    sensor->start(); // starting background thread

    // connect sensor and logic
    connect(sensor, &MockSensor::realTimeReading, logic, &StablePointLogic::realTimeReading, Qt::AutoConnection);
    connect(logic, &StablePointLogic::stopStreaming, sensor, &MockSensor::stopStreaming);
    connect(logic, &StablePointLogic::startStreaming, sensor, &MockSensor::startStreaming);

    // start stable point measurement
    logic->startStablePointMeasurement(config);

    // check if "real" measurement is triggert
    QSignalSpy spy_startMeasurement(logic, SIGNAL(startMeasurement()));

    qDebug() << "test 1";
    QCOMPARE(spy_startMeasurement.wait(3000), true);
    QCOMPARE(spy_startMeasurement.count(), 1);

    spy_startMeasurement.clear();
    qDebug() << "test 2";
    QCOMPARE(spy_startMeasurement.wait(2500), false);
    QCOMPARE(spy_startMeasurement.count(), 0);

    // start stable point measurement
    logic->startStablePointMeasurement(config);
    qDebug() << "test 3";
    QCOMPARE(spy_startMeasurement.wait(5000), true);
    QCOMPARE(spy_startMeasurement.count(), 1);

    logic->stopStablePointMeasurement();
    sensor->terminate();
    sensor->wait(1000);
}

QTEST_MAIN(StablePointTest) // instead of QTEST_APPLESS_MAIN because of QTimer

#include "stablepointtest.moc"
