#include <QString>
#include <QtTest>

#include "position.h"
#include "radius.h"
#include "featurewrapper.h"
#include "oivec.h"
#include "watchwindowutil.h"
#include "chooselalib.h"
#include "util.h"

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
    void testPoint2();

private:
    QPointer<FeatureWrapper> createFeature(FeatureTypes type, double x, double y, double z, double i, double j, double k);
    OiVec createTrackerXYZ(double x, double y, double z);
    void verifyResult(Result result,  double x, double y, double z, double dx, double dy, double dz, double d3D, double radius);
};

WatchwindowTest::WatchwindowTest()
{
}

/*
point	actual		Punkt	999.99	1999.99	2999.98	0.01	2/3	FastPoint	BestFitPoint		-/-	-/-	-/-	-/-
plane	actual		Ebene	1375.00	1625.00	1024.75	0.39	4/5	FastPoint	BestFitPlane		0.100818	-0.097854	0.990081	-/-
circle	actual		Kreis	600.01	500.00	1000.00	0.00	4/5	FastPoint	BestFitCircleInPlane		0.000010	0.000007	-1.000000	781.03
line	actual		Gerade	2979.99	3019.99	999.99	68.31	5/5	FastPoint	BestFitLine		0.699995	0.714147	0.000000	-/-
cylinder	actual		Zylinder	-25.00	-25.00	1142.85	0.01	7/8	FastPoint	BestFitCylinder		0.000006	-0.000009	1.000000	1025.30
*/

QPointer<FeatureWrapper> WatchwindowTest::createFeature(FeatureTypes type, double x, double y, double z, double i, double j, double k) {
    QPointer<FeatureWrapper> featurewrapper = new FeatureWrapper();

    OiVec xyz = OiVec(3);
    xyz.setAt(0, x);
    xyz.setAt(1, y);
    xyz.setAt(2, z);

    OiVec ijk = OiVec(3);
    switch(type) {
        case eCylinderFeature:
            ijk.setAt(0, i);
            ijk.setAt(1, j);
            ijk.setAt(2, k);
    }

    switch(type) {
        case ePointFeature:
            QPointer<Point> point = new Point(false, Position(xyz));
            point->setIsSolved(true);
            featurewrapper->setPoint(point);
        break;
    }


    return featurewrapper;
}

OiVec WatchwindowTest::createTrackerXYZ(double x, double y, double z) {
    OiVec trackerXYZ = OiVec(4);
    trackerXYZ.setAt(0, x);
    trackerXYZ.setAt(1, y);
    trackerXYZ.setAt(2, z);
    trackerXYZ.setAt(3, 1.0);
    return trackerXYZ;
}

void WatchwindowTest::verifyResult(Result result,  double x, double y, double z, double dx, double dy, double dz, double d3D, double radius) {
    OiVec resultPosition = result.position.getVector();
    double resultRadius = result.radius.getRadius();
    COMPARE_DOUBLE(resultPosition.getAt(0), x, 0.0001);
    COMPARE_DOUBLE(resultPosition.getAt(1), y, 0.0001);
    COMPARE_DOUBLE(resultPosition.getAt(2), z, 0.0001);
    if(resultRadius > 0) {
        COMPARE_DOUBLE(resultRadius, radius, 0.0001);
    }

    COMPARE_DOUBLE(result.d3D, d3D, 0.0001);
    COMPARE_DOUBLE(result.delta.getAt(0), dx, 0.0001);
    COMPARE_DOUBLE(result.delta.getAt(1), dy, 0.0001);
    COMPARE_DOUBLE(result.delta.getAt(2), dz, 0.0001);
}

void WatchwindowTest::testPoint1()
{

    ChooseLALib::setLinearAlgebra(ChooseLALib::Armadillo);

    WatchWindowUtil util;


    OiVec trackerXYZ = createTrackerXYZ(1.001, 1.002, 1.003);
    QPointer<FeatureWrapper> featurewrapper = createFeature(ePointFeature, 1.003, 1.005, 1.007, 0, 0, 0);
    Result result = util.getPosition(featurewrapper, trackerXYZ);

    verifyResult(result, 1.003, 1.005, 1.007, 0.002, 0.003, 0.004, 0.0054, 0.0);

}

void WatchwindowTest::testPoint2()
{

    ChooseLALib::setLinearAlgebra(ChooseLALib::Armadillo);

    WatchWindowUtil util;


    OiVec trackerXYZ = createTrackerXYZ(1000, 2000, 3000);
    QPointer<FeatureWrapper> featurewrapper = createFeature(ePointFeature, 999.99, 1999.99, 2999.98, 0, 0, 0);
    Result result = util.getPosition(featurewrapper, trackerXYZ);

    verifyResult(result, 999.99, 1999.99, 2999.98, -0.01, -0.01, -0.20, 0.0245, 0.0);

}

QTEST_APPLESS_MAIN(WatchwindowTest)

#include "tst_watchwindowtest.moc"
