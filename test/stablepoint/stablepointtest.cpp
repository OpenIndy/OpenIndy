#include <QString>
#include <QtTest>

#include <QPointer>

#include "position.h"
#include "oivec.h"
#include "chooselalib.h"
#include "util.h"
#include "stablepointlogic.h"
#include "oijob.h"

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

void StablePointTest::testStablePoint1()
{


    ChooseLALib::setLinearAlgebra(ChooseLALib::Armadillo);

    QPointer<OiJob> job;
    StablePointLogic logic = StablePointLogic(job);

    QTEST_ASSERT(false);

}


QTEST_APPLESS_MAIN(StablePointTest)

#include "stablepointtest.moc"
