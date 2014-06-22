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
    p1->xyz.setAt(0,49.9995977898474);
    p1->xyz.setAt(1,39.9996595954925);
    p1->xyz.setAt(2,2.99992723667164);
    Point *p2 = new Point(true);
    p2->setIsSolved(true);
    p2->setFeatureName("1");
    p2->xyz.setAt(0,9.99216332807491e-05);
    p2->xyz.setAt(1,9.98655165899318e-05);
    p2->xyz.setAt(2,9.98370710586951e-05);
    Point *p3 = new Point(false);
    p3->setIsSolved(true);
    p3->setFeatureName("2");
    p3->xyz.setAt(0,49.9998194058163);
    p3->xyz.setAt(1,49.9998151233339);
    p3->xyz.setAt(2,2.99994061683961);
    Point *p4 = new Point(true);
    p4->setIsSolved(true);
    p4->setFeatureName("2");
    p4->xyz.setAt(0,9.9999986610303573);
    p4->xyz.setAt(1,9.06443371866201e-05);
    p4->xyz.setAt(2,9.49767943204531e-05);
    Point *p5 = new Point(false);
    p5->setIsSolved(true);
    p5->setFeatureName("3");
    p5->xyz.setAt(0,39.9998531539475);
    p5->xyz.setAt(1,49.999863690377);
    p5->xyz.setAt(2,2.99995124921744);
    Point *p6 = new Point(true);
    p6->setIsSolved(true);
    p6->setFeatureName("3");
    p6->xyz.setAt(0,10.0000099674564);
    p6->xyz.setAt(1,10.0000276262958);
    p6->xyz.setAt(2,0.00013092089845747);
    Point *p7 = new Point(false);
    p7->setIsSolved(true);
    p7->setFeatureName("4");
    p7->xyz.setAt(0,39.9995027431988);
    p7->xyz.setAt(1,39.9995881257577);
    p7->xyz.setAt(2,2.99994542562235);
    Point *p8 = new Point(true);
    p8->setIsSolved(true);
    p8->setFeatureName("4");
    p8->xyz.setAt(0,9.23218327813622e-05);
    p8->xyz.setAt(1,9.99998029800553);
    p8->xyz.setAt(2,9.98473982983145e-05);


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
