#include <QString>
#include <QtTest>

#include <QPointer>
#include <QList>

#include "controllersensoractions.h"
#include "featuretablemodel.h"

#include "testcontrollersensoractions.h"
#include "testmeasurebehaviorlogic.h"
#include "measurebehaviortypes.h"

using namespace oi;

class MeasureBehaviorTest : public QObject
{
    Q_OBJECT

public:
    MeasureBehaviorTest();

private Q_SLOTS:
    void testSearch();
    void testSkip();
    void testAimAndMeasure();

};

MeasureBehaviorTest::MeasureBehaviorTest()
{
}

// basic test
void MeasureBehaviorTest::testAimAndMeasure()
{

    TestMeasureBehaviorLogic logic;
    TestControllerSensorActions controller;
    QPointer<FeatureTableModel> featureTableModel;

    QList<int> measureFeatures;

    measureFeatures.append(101);
    measureFeatures.append(102);
    measureFeatures.append(103);

    logic.init(&controller, measureFeatures, featureTableModel);
    QCOMPARE(logic.next(), true);
    logic.measure();
    logic.sensorActionFinished(true, SensorWorkerMessage::MEASUREMENT_FINISHED); // calls next / measure
    logic.sensorActionFinished(true, SensorWorkerMessage::MEASUREMENT_FINISHED); // calls next / measure
    logic.sensorActionFinished(true, SensorWorkerMessage::MEASUREMENT_FINISHED); // calls next / measure

    QCOMPARE(logic.next(), false);
    QCOMPARE(logic.featureIds.size(), 3);
    QCOMPARE(controller.actions.join(","), QString("startAim,startMeasurement,startAim,startMeasurement,startAim,startMeasurement"));

}

void MeasureBehaviorTest::testSkip()
{

    TestMeasureBehaviorLogic logic;
    TestControllerSensorActions controller;
    QPointer<FeatureTableModel> featureTableModel;

    QList<int> measureFeatures;

    measureFeatures.append(101);
    measureFeatures.append(102);
    measureFeatures.append(103);

    logic.init(&controller, measureFeatures, featureTableModel);
    QCOMPARE(logic.next(), true);
    logic.measure();
    logic.sensorActionFinished(true, SensorWorkerMessage::MEASUREMENT_FINISHED); // calls next / measure

    logic.setDecision(Decision::eSkip);
    logic.sensorActionFinished(false, SensorWorkerMessage::FAILED_TO_MEASURE);

    logic.sensorActionFinished(true, SensorWorkerMessage::MEASUREMENT_FINISHED); // calls next / measure

    QCOMPARE(logic.next(), false);
    QCOMPARE(logic.featureIds.size(), 3);
    QCOMPARE(controller.actions.join(","), QString("startAim,startMeasurement,startAim,startMeasurement,startAim,startMeasurement"));
}

void MeasureBehaviorTest::testSearch()
{

    TestMeasureBehaviorLogic logic;
    TestControllerSensorActions controller;
    QPointer<FeatureTableModel> featureTableModel;

    QList<int> measureFeatures;

    measureFeatures.append(101);
    measureFeatures.append(102);
    measureFeatures.append(103);

    logic.init(&controller, measureFeatures, featureTableModel);
    QCOMPARE(logic.next(), true);
    logic.measure();
    logic.sensorActionFinished(true, SensorWorkerMessage::MEASUREMENT_FINISHED); // calls next / measure

    logic.setDecision(Decision::eSearch);
    logic.sensorActionFinished(false, SensorWorkerMessage::FAILED_TO_MEASURE);

    logic.sensorActionFinished(true, SensorWorkerMessage::MEASUREMENT_FINISHED); // calls next / measure

    QCOMPARE(logic.next(), false);
    QCOMPARE(logic.featureIds.size(), 3);
    QCOMPARE(controller.actions.join(","), QString("startAim,startMeasurement,startAim,startMeasurement,startSearch,startMeasurement,startAim,startMeasurement"));

}

QTEST_MAIN(MeasureBehaviorTest) // instead of QTEST_APPLESS_MAIN because of: "QWidget: Must construct a QApplication before a QWidget"

#include "measurebehaviortest.moc"
