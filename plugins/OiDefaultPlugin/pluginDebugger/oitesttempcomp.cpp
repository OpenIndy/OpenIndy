#include "oitesttempcomp.h"
#include "chooselalib.h"

void OiTestTempComp::initTestCase()
{
    qDebug() <<"-------start testint temp comp-----";
}

void OiTestTempComp::testTempComp()
{
    ChooseLALib::setLinearAlgebra(ChooseLALib::Armadillo);
    qDebug() << "test temp comp";

    Point *p1 = new Point(false);
    p1->setIsSolved(true);
    p1->setFeatureName("1");
    p1->xyz.setAt(0,50.0001);
    p1->xyz.setAt(1,40.0002);
    p1->xyz.setAt(2,3.0);
    Point *p2 = new Point(true);
    p2->setIsSolved(true);
    p2->setFeatureName("1");
    p2->xyz.setAt(0,0.0);
    p2->xyz.setAt(1,0.0);
    p2->xyz.setAt(2,0.0);
    Point *p3 = new Point(false);
    p3->setIsSolved(true);
    p3->setFeatureName("2");
    p3->xyz.setAt(0,50.0);
    p3->xyz.setAt(1,50.0);
    p3->xyz.setAt(2,2.99997);
    Point *p4 = new Point(true);
    p4->setIsSolved(true);
    p4->setFeatureName("2");
    p4->xyz.setAt(0,10.0);
    p4->xyz.setAt(1,0.0);
    p4->xyz.setAt(2,0.0);
    Point *p5 = new Point(false);
    p5->setIsSolved(true);
    p5->setFeatureName("3");
    p5->xyz.setAt(0,40.0);
    p5->xyz.setAt(1,50.0001);
    p5->xyz.setAt(2,3.00015);
    Point *p6 = new Point(true);
    p6->setIsSolved(true);
    p6->setFeatureName("3");
    p6->xyz.setAt(0,10.0);
    p6->xyz.setAt(1,10.0);
    p6->xyz.setAt(2,0.0);
    Point *p7 = new Point(false);
    p7->setIsSolved(true);
    p7->setFeatureName("4");
    p7->xyz.setAt(0,39.9999);
    p7->xyz.setAt(1,40.0);
    p7->xyz.setAt(2,3.0);
    Point *p8 = new Point(true);
    p8->setIsSolved(true);
    p8->setFeatureName("4");
    p8->xyz.setAt(0,0.0);
    p8->xyz.setAt(1,10.0);
    p8->xyz.setAt(2,0.0);


    /*Function *function = new ExtendedTemperatureCompensation();
    function->addPoint(p1,0);
    function->addPoint(p2,0);
    function->addPoint(p3,0);
    function->addPoint(p4,0);
    function->addPoint(p5,0);
    function->addPoint(p6,0);
    function->addPoint(p7,0);
    function->addPoint(p8,0);*/

    ExtendedTemperatureCompensation *f = new ExtendedTemperatureCompensation();
    f->addPoint(p1,0);
    f->addPoint(p2,0);
    f->addPoint(p3,0);
    f->addPoint(p4,0);
    f->addPoint(p5,0);
    f->addPoint(p6,0);
    f->addPoint(p7,0);
    f->addPoint(p8,0);
    QList<Point> start;
    start.append(*p1);
    start.append(*p3);
    start.append(*p5);
    start.append(*p7);
    QList<Point> dest;
    dest.append(*p2);
    dest.append(*p4);
    dest.append(*p6);
    dest.append(*p8);
    f->points_startSystem.append(start);
    f->points_targetSystem.append(dest);


    TrafoParam *tp = new TrafoParam();

    QVERIFY2(f->exec(*tp),"failed");
}

void OiTestTempComp::cleanupTestCase()
{
    qDebug() << "-------------finished temp comp--------";
}
