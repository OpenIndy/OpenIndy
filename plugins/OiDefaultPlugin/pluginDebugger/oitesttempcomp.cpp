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

    ScalarEntityTemperature *s = new ScalarEntityTemperature(false);
    s->setFeatureName("testtemp");
    s->setCommonState(false);
    s->setIsSolved(true);
    s->setTemperature(23.0);

    Function *function = new SimpleTemperatureCompensation();
    function->addScalarEntityTemperature(s,0);

    TrafoParam *tp = new TrafoParam();

    QVERIFY2(function->exec(*tp),"failed");
}

void OiTestTempComp::cleanupTestCase()
{
    qDebug() << "-------------finished temp comp--------";
}
