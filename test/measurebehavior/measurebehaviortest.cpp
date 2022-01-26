#include <QString>
#include <QtTest>

#include <QPointer>

#include "measurebehaviordialog.h"
#include "measurebehaviorlogic.h"

//using namespace oi;
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

    MeasureBehaviorLogic logic;

}

QTEST_MAIN(MeasureBehaviorTest) // instead of QTEST_APPLESS_MAIN because of QTimer

#include "measurebehaviortest.moc"
