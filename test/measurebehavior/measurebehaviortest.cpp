#include <QString>
#include <QtTest>

#include <QPointer>
#include <QList>

#include "ControllerSensorActions.h"
#include "featuretablemodel.h"

#include "testcontrollersensoractions.h"
#include "testmeasurebehaviorlogic.h"

using namespace oi;
//using namespace oi::math;


class MeasureBehaviorTest : public QObject
{
    Q_OBJECT

public:
    MeasureBehaviorTest();

private Q_SLOTS:
    void testAimAndSearch();

};

MeasureBehaviorTest::MeasureBehaviorTest()
{
}

// basic test
void MeasureBehaviorTest::testAimAndSearch()
{

    TestMeasureBehaviorLogic logic;
    TestControllerSensorActions controller;
    QPointer<FeatureTableModel> featureTableModel;

    QList<int> measureFeatures;

    measureFeatures.append(101);
    measureFeatures.append(102);
    measureFeatures.append(103);
    measureFeatures.append(104);
    measureFeatures.append(105);

    logic.init(&controller, measureFeatures, featureTableModel);
    logic.measure();

    qDebug() << "done";

}

QTEST_MAIN(MeasureBehaviorTest) // instead of QTEST_APPLESS_MAIN because of: "QWidget: Must construct a QApplication before a QWidget"

#include "measurebehaviortest.moc"
