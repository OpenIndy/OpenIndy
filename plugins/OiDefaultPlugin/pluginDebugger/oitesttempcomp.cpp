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
    p1->xyz.setAt(0,10.0);
    p1->xyz.setAt(1,10.0);
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
    p3->xyz.setAt(0,10.0);
    p3->xyz.setAt(1,20.05);
    p3->xyz.setAt(2,3.0);
    Point *p4 = new Point(true);
    p4->setIsSolved(true);
    p4->setFeatureName("2");
    p4->xyz.setAt(0,10.0);
    p4->xyz.setAt(1,0.0);
    p4->xyz.setAt(2,0.0);

    Point *p5 = new Point(false);
    p5->setIsSolved(true);
    p5->setFeatureName("3");
    p5->xyz.setAt(0,2.08);
    p5->xyz.setAt(1,20.05);
    p5->xyz.setAt(2,3.0);
    Point *p6 = new Point(true);
    p6->setIsSolved(true);
    p6->setFeatureName("3");
    p6->xyz.setAt(0,10.0);
    p6->xyz.setAt(1,8.0);
    p6->xyz.setAt(2,0.0);

    Point *p7 = new Point(false);
    p7->setIsSolved(true);
    p7->setFeatureName("4");
    p7->xyz.setAt(0,2.08);
    p7->xyz.setAt(1,10.0);
    p7->xyz.setAt(2,3.0);
    Point *p8 = new Point(true);
    p8->setIsSolved(true);
    p8->setFeatureName("4");
    p8->xyz.setAt(0,0.0);
    p8->xyz.setAt(1,8.0);
    p8->xyz.setAt(2,0.0);

    /*Point *p9 = new Point(false);
    p9->setIsSolved(true);
    p9->setFeatureName("5");
    p9->xyz.setAt(0,3.73823);
    p9->xyz.setAt(1,-0.842572);
    p9->xyz.setAt(2,0.00970864);
    Point *p10 = new Point(true);
    p10->setIsSolved(true);
    p10->setFeatureName("5");
    p10->xyz.setAt(0,0.024491);
    p10->xyz.setAt(1,5.1589);
    p10->xyz.setAt(2,1.51004);

    Point *p11 = new Point(false);
    p11->setIsSolved(true);
    p11->setFeatureName("6");
    p11->xyz.setAt(0,3.83759);
    p11->xyz.setAt(1,-2.12246);
    p11->xyz.setAt(2,0.641102);
    Point *p12 = new Point(true);
    p12->setIsSolved(true);
    p12->setFeatureName("6");
    p12->xyz.setAt(0,0.020776);
    p12->xyz.setAt(1,6.44673);
    p12->xyz.setAt(2,2.13286);

    Point *p13 = new Point(false);
    p13->setIsSolved(true);
    p13->setFeatureName("7");
    p13->xyz.setAt(0,3.83786);
    p13->xyz.setAt(1,-2.11838);
    p13->xyz.setAt(2,0.00702066);
    Point *p14 = new Point(true);
    p14->setIsSolved(true);
    p14->setFeatureName("7");
    p14->xyz.setAt(0,0.02348);
    p14->xyz.setAt(1,6.4385);
    p14->xyz.setAt(2,1.49886);

    Point *p15 = new Point(false);
    p15->setIsSolved(true);
    p15->setFeatureName("8");
    p15->xyz.setAt(0,3.83685);
    p15->xyz.setAt(1,-2.11016);
    p15->xyz.setAt(2,-0.725363);
    Point *p16 = new Point(true);
    p16->setIsSolved(true);
    p16->setFeatureName("8");
    p16->xyz.setAt(0,0.027691);
    p16->xyz.setAt(1,6.42539);
    p16->xyz.setAt(2,0.766588);*/


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
    /*f->addPoint(p9,0);
    f->addPoint(p10,0);
    f->addPoint(p11,0);
    f->addPoint(p12,0);
    f->addPoint(p13,0);
    f->addPoint(p14,0);
    f->addPoint(p15,0);
    f->addPoint(p16,0);*/
    QList<Point> start;
    start.append(*p1);
    start.append(*p3);
    start.append(*p5);
    start.append(*p7);
    /*start.append(*p9);
    start.append(*p11);
    start.append(*p13);
    start.append(*p15);*/
    QList<Point> dest;
    dest.append(*p2);
    dest.append(*p4);
    dest.append(*p6);
    dest.append(*p8);
    /*dest.append(*p10);
    dest.append(*p12);
    dest.append(*p14);
    dest.append(*p16);*/
    f->points_startSystem.append(start);
    f->points_targetSystem.append(dest);


    TrafoParam *tp = new TrafoParam();

    QVERIFY2(f->exec(*tp),"failed");
}

void OiTestTempComp::cleanupTestCase()
{
    qDebug() << "-------------finished temp comp--------";
}
