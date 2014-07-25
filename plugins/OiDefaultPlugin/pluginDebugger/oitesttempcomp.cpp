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
    p1->xyz.setAt(0,2.20441);
    p1->xyz.setAt(1,-3.31189);
    p1->xyz.setAt(2,0.774685);
    Point *p2 = new Point(true);
    p2->setIsSolved(true);
    p2->setFeatureName("1");
    p2->xyz.setAt(0,0.015845);
    p2->xyz.setAt(1,1.43826);
    p2->xyz.setAt(2,2.13013);

    Point *p3 = new Point(false);
    p3->setIsSolved(true);
    p3->setFeatureName("2");
    p3->xyz.setAt(0,2.2052);
    p3->xyz.setAt(1,-3.30304);
    p3->xyz.setAt(2,-0.582934);
    Point *p4 = new Point(true);
    p4->setIsSolved(true);
    p4->setFeatureName("2");
    p4->xyz.setAt(0,0.025035);
    p4->xyz.setAt(1,1.43067);
    p4->xyz.setAt(2,0.771385);

    Point *p5 = new Point(false);
    p5->setIsSolved(true);
    p5->setFeatureName("3");
    p5->xyz.setAt(0,1.44551);
    p5->xyz.setAt(1,-3.51086);
    p5->xyz.setAt(2,0.147876);
    Point *p6 = new Point(true);
    p6->setIsSolved(true);
    p6->setFeatureName("3");
    p6->xyz.setAt(0,0.022824);
    p6->xyz.setAt(1,2.21868);
    p6->xyz.setAt(2,1.50151);

    Point *p7 = new Point(false);
    p7->setIsSolved(true);
    p7->setFeatureName("4");
    p7->xyz.setAt(0,-0.174336);
    p7->xyz.setAt(1,-3.94954);
    p7->xyz.setAt(2,0.149645);
    Point *p8 = new Point(true);
    p8->setIsSolved(true);
    p8->setFeatureName("4");
    p8->xyz.setAt(0,0.021866);
    p8->xyz.setAt(1,3.89655);
    p8->xyz.setAt(2,1.50377);

    Point *p9 = new Point(false);
    p9->setIsSolved(true);
    p9->setFeatureName("5");
    p9->xyz.setAt(0,-1.39393);
    p9->xyz.setAt(1,-4.27637);
    p9->xyz.setAt(2,0.156076);
    Point *p10 = new Point(true);
    p10->setIsSolved(true);
    p10->setFeatureName("5");
    p10->xyz.setAt(0,0.024491);
    p10->xyz.setAt(1,5.1589);
    p10->xyz.setAt(2,1.51004);

    Point *p11 = new Point(false);
    p11->setIsSolved(true);
    p11->setFeatureName("6");
    p11->xyz.setAt(0,-2.63578);
    p11->xyz.setAt(1,-4.61629);
    p11->xyz.setAt(2,0.77887);
    Point *p12 = new Point(true);
    p12->setIsSolved(true);
    p12->setFeatureName("6");
    p12->xyz.setAt(0,0.020776);
    p12->xyz.setAt(1,6.44673);
    p12->xyz.setAt(2,2.13286);

    Point *p13 = new Point(false);
    p13->setIsSolved(true);
    p13->setFeatureName("7");
    p13->xyz.setAt(0,-2.62897);
    p13->xyz.setAt(1,-4.61111);
    p13->xyz.setAt(2,0.144917);
    Point *p14 = new Point(true);
    p14->setIsSolved(true);
    p14->setFeatureName("7");
    p14->xyz.setAt(0,0.02348);
    p14->xyz.setAt(1,6.4385);
    p14->xyz.setAt(2,1.49886);

    Point *p15 = new Point(false);
    p15->setIsSolved(true);
    p15->setFeatureName("8");
    p15->xyz.setAt(0,-2.61738);
    p15->xyz.setAt(1,-4.60377);
    p15->xyz.setAt(2,-0.586624);
    Point *p16 = new Point(true);
    p16->setIsSolved(true);
    p16->setFeatureName("8");
    p16->xyz.setAt(0,0.027691);
    p16->xyz.setAt(1,6.42539);
    p16->xyz.setAt(2,0.766588);


    /*Function *function = new ExtendedTemperatureCompensation();
    function->addPoint(p1,0);
    function->addPoint(p2,0);
    function->addPoint(p3,0);
    function->addPoint(p4,0);
    function->addPoint(p5,0);
    function->addPoint(p6,0);
    function->addPoint(p7,0);
    function->addPoint(p8,0);*/

    Helmert6Param *f = new Helmert6Param();
    f->addPoint(p1,0);
    f->addPoint(p2,0);
    f->addPoint(p3,0);
    f->addPoint(p4,0);
    f->addPoint(p5,0);
    f->addPoint(p6,0);
    f->addPoint(p7,0);
    f->addPoint(p8,0);
    f->addPoint(p9,0);
    f->addPoint(p10,0);
    f->addPoint(p11,0);
    f->addPoint(p12,0);
    f->addPoint(p13,0);
    f->addPoint(p14,0);
    f->addPoint(p15,0);
    f->addPoint(p16,0);
    QList<Point> start;
    start.append(*p1);
    start.append(*p3);
    start.append(*p5);
    start.append(*p7);
    start.append(*p9);
    start.append(*p11);
    start.append(*p13);
    start.append(*p15);
    QList<Point> dest;
    dest.append(*p2);
    dest.append(*p4);
    dest.append(*p6);
    dest.append(*p8);
    dest.append(*p10);
    dest.append(*p12);
    dest.append(*p14);
    dest.append(*p16);
    f->points_startSystem.append(start);
    f->points_targetSystem.append(dest);


    TrafoParam *tp = new TrafoParam();
    tp->setFeatureName("testparam");

    CoordinateSystem *cs1 = new CoordinateSystem();
    CoordinateSystem *cs2 = new CoordinateSystem();

    cs1->setFeatureName("cs1");
    cs2->setFeatureName("cs2");

    TrafoParam *t = new TrafoParam();
    t->setFeatureName("movement");
    t->setCoordinateSystems(cs1,cs1);
    t->setIsMovement(true);
    t->setIsUsed(true);

    tp->setCoordinateSystems(cs1,cs2);

    cs1->addTransformationParameter(t);

    QVERIFY2(f->exec(*tp),"failed");
}

void OiTestTempComp::cleanupTestCase()
{
    qDebug() << "-------------finished temp comp--------";
}
