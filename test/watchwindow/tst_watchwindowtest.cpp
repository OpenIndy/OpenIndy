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
#define _OI_VEC(v) v.getAt(0) << "," << v.getAt(1) << "," << v.getAt(2)
#define DEBUG_RESULT(result)  qDebug() << qSetRealNumberPrecision(10) << "\nx, y, z, dx, dy, dz, d3D, radius\n" << _OI_VEC(result.position.getVector()) << "," << _OI_VEC(result.delta) << "," << result.d3D << "," << result.radius.getRadius();

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

    void testLine1();
    void testPlane1();
    void testPlane2();
    void testPlane3();
    void testCircle1();
    void testCylinder1();

private:
    QPointer<FeatureWrapper> createFeature(FeatureTypes type, double x, double y, double z, double i, double j, double k, double radius);
    OiVec createTrackerXYZ(double x, double y, double z);
    void verifyResult(Result result,  double x, double y, double z, double dx, double dy, double dz, double d3D, double radius);
};

WatchwindowTest::WatchwindowTest()
{
}

QPointer<FeatureWrapper> WatchwindowTest::createFeature(FeatureTypes type, double x, double y, double z, double i, double j, double k, double radius) {
    QPointer<FeatureWrapper> featurewrapper = new FeatureWrapper();

    OiVec xyz = OiVec(3);
    xyz.setAt(0, x);
    xyz.setAt(1, y);
    xyz.setAt(2, z);

    OiVec ijk = OiVec(3);
    switch(type) {
    case eLineFeature:
    case ePlaneFeature:
    case eCircleFeature:
    case eCylinderFeature:
        ijk.setAt(0, i);
        ijk.setAt(1, j);
        ijk.setAt(2, k);
        break;
    }

    switch(type) {
    case ePointFeature: {
            QPointer<Point> point = new Point(false, Position(xyz));
            point->setIsSolved(true);
            featurewrapper->setPoint(point);
            break;
        }
    case eLineFeature: {
            QPointer<Line> line = new Line(false, Position(xyz), Direction(ijk));
            line->setIsSolved(true);
            featurewrapper->setLine(line);
        break;
    }
    case ePlaneFeature: {
            QPointer<Plane> plane = new Plane(false, Position(xyz), Direction(ijk));
            plane->setIsSolved(true);
            featurewrapper->setPlane(plane);
        break;
    }
    case eCircleFeature:{
            QPointer<Circle> circle = new Circle(false, Position(xyz), Direction(ijk), Radius(radius));
            circle->setIsSolved(true);
            featurewrapper->setCircle(circle);
        break;
    }
    case eCylinderFeature: {
            QPointer<Cylinder> cylinder = new Cylinder(false, Position(xyz), Direction(ijk), Radius(radius));
            cylinder->setIsSolved(true);
            featurewrapper->setCylinder(cylinder);
        break;
    }
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
    COMPARE_DOUBLE(resultPosition.getAt(0), x, 0.001);
    COMPARE_DOUBLE(resultPosition.getAt(1), y, 0.001);
    COMPARE_DOUBLE(resultPosition.getAt(2), z, 0.001);
    if(resultRadius > 0) {
        COMPARE_DOUBLE(resultRadius, radius, 0.001);
    }

    COMPARE_DOUBLE(result.d3D, d3D, 0.001);
    COMPARE_DOUBLE(result.delta.getAt(0), dx, 0.001);
    COMPARE_DOUBLE(result.delta.getAt(1), dy, 0.001);
    COMPARE_DOUBLE(result.delta.getAt(2), dz, 0.001);
}

void WatchwindowTest::testPoint1()
{

    ChooseLALib::setLinearAlgebra(ChooseLALib::Armadillo);

    WatchWindowUtil util;


    OiVec trackerXYZ = createTrackerXYZ(1.001, 1.002, 1.003);
    QPointer<FeatureWrapper> featurewrapper = createFeature(ePointFeature, 1.003, 1.005, 1.007, 0, 0, 0, 0);
    Result result = util.getPosition(featurewrapper, trackerXYZ);

    DEBUG_RESULT(result);

    verifyResult(result, 1.003, 1.005, 1.007, -0.002, -0.003, -0.004, 0.0054, 0.0);

}

void WatchwindowTest::testPoint2()
{

    ChooseLALib::setLinearAlgebra(ChooseLALib::Armadillo);

    WatchWindowUtil util;


    OiVec trackerXYZ = createTrackerXYZ(1000, 2000, 3000);
    QPointer<FeatureWrapper> featurewrapper = createFeature(ePointFeature, 999.99, 1999.99, 2999.98, 0, 0, 0, 0);
    Result result = util.getPosition(featurewrapper, trackerXYZ);

    DEBUG_RESULT(result);

    verifyResult(result, 999.99 , 1999.99 , 2999.98 , 0.01 , 0.01 , 0.02 , 0.0244, 0);

}

void WatchwindowTest::testLine1()
{

    ChooseLALib::setLinearAlgebra(ChooseLALib::Armadillo);

    WatchWindowUtil util;

    OiVec trackerXYZ = createTrackerXYZ(2000.1, 2000.1, 1000.1);
    QPointer<FeatureWrapper> featurewrapper = createFeature(eLineFeature, 2979.99, 3019.99, 999.99, 0.699995, 0.714147, 0.000000, 0);
    Result result = util.getPosition(featurewrapper, trackerXYZ);

    DEBUG_RESULT(result);

    verifyResult(result, 1990.008432 , 2009.993691 , 999.99 , 10.09156772 , -9.893690538 , 0.11 , 14.13283239 , 0);

}

void WatchwindowTest::testPlane1()
{

    ChooseLALib::setLinearAlgebra(ChooseLALib::Armadillo);

    WatchWindowUtil util;

    OiVec trackerXYZ = createTrackerXYZ(1500.0, 1500.0, 1002.00);
    QPointer<FeatureWrapper> featurewrapper = createFeature(ePlaneFeature, 1375.00, 1625.00, 1024.75, 0.100818, -0.097854, 0.990081, 0);
    Result result = util.getPosition(featurewrapper, trackerXYZ);

    DEBUG_RESULT(result);

    verifyResult(result,1499.767145 , 1500.226009 , 999.7132522 , 0.2328550246 , -0.2260092005 , 2.28674776 , 2.30965732 , 0);

}

void WatchwindowTest::testPlane2()
{

    ChooseLALib::setLinearAlgebra(ChooseLALib::Armadillo);

    WatchWindowUtil util;

    OiVec trackerXYZ = createTrackerXYZ(1.0, 1.0, -1.00);
    QPointer<FeatureWrapper> featurewrapper = createFeature(ePlaneFeature, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0);
    Result result = util.getPosition(featurewrapper, trackerXYZ);

    DEBUG_RESULT(result);

    verifyResult(result, 1 , 1 , 0 , 0 , 0 , -1 , -1 , 0);

}

void WatchwindowTest::testPlane3()
{

    ChooseLALib::setLinearAlgebra(ChooseLALib::Armadillo);

    WatchWindowUtil util;

    OiVec trackerXYZ = createTrackerXYZ(1.0, 1.0, 1.00);
    QPointer<FeatureWrapper> featurewrapper = createFeature(ePlaneFeature, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0);
    Result result = util.getPosition(featurewrapper, trackerXYZ);

    DEBUG_RESULT(result);

    verifyResult(result, 1 , 1 , 0 , 0 , 0 , 1 , 1 , 0);

}

void WatchwindowTest::testCircle1()
{

    ChooseLALib::setLinearAlgebra(ChooseLALib::Armadillo);

    WatchWindowUtil util;

    OiVec trackerXYZ = createTrackerXYZ(0., 1001., 1000.0);
    QPointer<FeatureWrapper> featurewrapper = createFeature(eCircleFeature, 600.01, 500.00, 1000.00, 0.000010, 0.000007, -1.000000, 781.03);
    Result result = util.getPosition(featurewrapper, trackerXYZ);

    DEBUG_RESULT(result);

    verifyResult(result, 600.01 , 500 , 1000.002493 , -600.01 , 501 , -0.0024931 , 0.6432054342 , 781.03);

}

void WatchwindowTest::testCylinder1()
{

    ChooseLALib::setLinearAlgebra(ChooseLALib::Armadillo);

    WatchWindowUtil util;

    OiVec trackerXYZ = createTrackerXYZ(2, 1002, 1003);
    QPointer<FeatureWrapper> featurewrapper = createFeature(eCylinderFeature, -25.00, -25.00, 1142.85, 0.000006, -0.000009, 1.000000, 1025.30 );
    Result result = util.getPosition(featurewrapper, trackerXYZ);

    DEBUG_RESULT(result);

    verifyResult(result, -25.00083915 , -24.99874127 , 1002.990919 , 27.00083915 , 1026.998741 , 0.009081 , 2.053619726 , 1025.3);

}


QTEST_APPLESS_MAIN(WatchwindowTest)

#include "tst_watchwindowtest.moc"
