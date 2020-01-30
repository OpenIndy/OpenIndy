#include <QString>
#include <QtTest>

#include "position.h"
#include "radius.h"
#include "featurewrapper.h"
#include "oivec.h"
#include "watchwindowutil.h"
#include "chooselalib.h"

#define COMPARE_DOUBLE(actual, expected, threshold) QVERIFY(std::abs(actual-expected)< threshold);

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

    ChooseLALib::setLinearAlgebra(ChooseLALib::Armadillo);

    WatchWindowUtil util;

    OiVec trackerXYZ = OiVec(4);
    trackerXYZ.setAt(0, 1.001);
    trackerXYZ.setAt(1, 1.002);
    trackerXYZ.setAt(2, 1.003);
    trackerXYZ.setAt(3, 1.0);

    OiVec pos = OiVec(3);
    pos.setAt(0, 1.003);
    pos.setAt(1, 1.005);
    pos.setAt(2, 1.007);

    QPointer<Point> point = new Point(false, Position(pos));
    point->setIsSolved(true);

    QPointer<FeatureWrapper> featurewrapper = new FeatureWrapper();
    featurewrapper->setPoint(point);

    Result result = util.getPosition(featurewrapper, trackerXYZ);

    OiVec resultPosition = result.position.getVector();
    double resultRadius = result.radius.getRadius();
    QVERIFY2(resultPosition.getAt(0) == 1.003, "");
    QVERIFY2(resultPosition.getAt(1) == 1.005, "");
    QVERIFY2(resultPosition.getAt(2) == 1.007, "");
    QVERIFY2(resultRadius == 0, "radius");

    COMPARE_DOUBLE(result.d3D, 0.0054, 0.0001);
    COMPARE_DOUBLE(result.delta.getAt(0), 0.002, 0.0001);
    COMPARE_DOUBLE(result.delta.getAt(1), 0.003, 0.0001);
    COMPARE_DOUBLE(result.delta.getAt(2), 0.004, 0.0001);


}

QTEST_APPLESS_MAIN(WatchwindowTest)

#include "tst_watchwindowtest.moc"
