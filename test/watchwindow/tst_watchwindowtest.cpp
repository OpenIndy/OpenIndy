#include <QString>
#include <QtTest>

#include <position.h>
#include <radius.h>
#include <featurewrapper.h>
#include <oivec.h>
#include <watchwindowutil.h>

using namespace oi;
using namespace oi::math;


class WatchwindowTest : public QObject
{
    Q_OBJECT

public:
    WatchwindowTest();

private Q_SLOTS:
    void testPoint1();
};

WatchwindowTest::WatchwindowTest()
{
}

void WatchwindowTest::testPoint1()
{

    WatchWindowUtil util;

    OiVec trackerXYZ = OiVec(3);
    trackerXYZ.setAt(0, 1.001);
    trackerXYZ.setAt(1, 1.002);
    trackerXYZ.setAt(2, 1.003);

    OiVec pos = OiVec(3);
    pos.setAt(0, 1.003);
    pos.setAt(1, 1.004);
    pos.setAt(2, 1.005);

    QPointer<Point> point = new Point(false, Position(pos));
    point->setIsSolved(true);

    QPointer<FeatureWrapper> featurewrapper = new FeatureWrapper();
    featurewrapper->setPoint(point);

    QPair<Position, Radius> result = util.getPosition(featurewrapper, trackerXYZ);

    OiVec resultPosition = result.first.getVector();
    double resultRadius = result.second.getRadius();
    QVERIFY2(resultPosition.getAt(0) == 1.003, "");
    QVERIFY2(resultPosition.getAt(1) == 1.004, "");
    QVERIFY2(resultPosition.getAt(2) == 1.005, "");

    QVERIFY2(resultRadius == 0, "raius");
}

QTEST_APPLESS_MAIN(WatchwindowTest)

#include "tst_watchwindowtest.moc"
