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
110.01 200.01 300.01\n\
108.01 200.01 300.01\n\
106.01 200.01 300.01\n\
104.01 200.01 300.01\n\
102.21 200.01 300.01\n\
# stable phase \n\
100.12 200.01 300.01\n\
100.13 200.01 300.01\n\
100.01 200.01 300.01\n\
100.03 200.01 300.01\n\
100.04 200.01 300.01\n\
##\n\
100.05 200.01 300.01\n\
100.01 200.01 300.01\n\
100.03 200.01 300.01\n\
100.04 200.01 300.01");


    MeasurementConfig config;
    config.setIsStablePoint(true);
    config.setStablePointThresholdTime(1.); // [second]
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

    QCOMPARE(spy_startMeasurement.wait(3000), true);
    QCOMPARE(spy_startMeasurement.count(), 1);


    logic->stopStablePointMeasurement();

}

void StablePointTest::testStablePoint_move_stable_move_stable()
{

    ChooseLALib::setLinearAlgebra(ChooseLALib::Armadillo);

    // readings from sensor
    QString readings("\
# 1 second: move phase, step 2 mm\n\
110.01 200.01 300.01\n\
108.01 200.01 300.01\n\
106.01 200.01 300.01\n\
104.01 200.01 300.01\n\
102.21 200.01 300.01\n\
# 3 seconds: stable phase\n\
100.12 200.01 300.01\n\
100.13 200.01 300.01\n\
100.01 200.01 300.01\n\
100.03 200.01 300.01\n\
100.04 200.01 300.01\n\
##\n\
100.05 200.01 300.01\n\
100.01 200.01 300.01\n\
100.03 200.01 300.01\n\
100.04 200.01 300.01\n\
100.01 200.01 300.01\n\
##\n\
100.03 200.01 300.01\n\
100.04 200.01 300.01\n\
100.05 200.01 300.01\n\
100.01 200.01 300.01\n\
100.03 200.01 300.01\n\
# move phase, step 2 mm\n\
100.01 201.91 300.01\n\
100.01 202.01 300.01\n\
100.01 204.01 300.01\n\
100.01 206.01 300.01\n\
100.01 208.01 300.01\n\
# stable phase\n\
100.35 210.01 300.01\n\
100.31 210.01 300.01\n\
100.32 210.01 300.01\n\
100.33 210.01 300.01\n\
100.34 210.01 300.01\n\
##\n\
100.35 210.01 300.01\n\
100.31 210.01 300.01\n\
100.31 210.01 300.01\n\
100.32 210.01 300.01");

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

}

QTEST_MAIN(StablePointTest) // instead of QTEST_APPLESS_MAIN because of QTimer

#include "stablepointtest.moc"
