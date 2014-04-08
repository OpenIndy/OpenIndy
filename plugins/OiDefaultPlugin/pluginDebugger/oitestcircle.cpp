#include "oitestcircle.h"
#include "chooselalib.h"
void OiTestCircle::initTestCase(){
    qDebug() << "********* Start testing of OiTestCircle *********";
}

void OiTestCircle::testLoeslerCircle(){
    ChooseLALib::setLinearAlgebra(ChooseLALib::Armadillo);

    qDebug() << "set pseudo observations";

    Observation *obs1 = new Observation(NULL,NULL);
    Observation *obs2 = new Observation(NULL,NULL);
    Observation *obs3 = new Observation(NULL,NULL);
    Observation *obs4 = new Observation(NULL,NULL);
    Observation *obs5 = new Observation(NULL,NULL);
    Observation *obs6 = new Observation(NULL,NULL);
    Observation *obs7 = new Observation(NULL,NULL);
    Observation *obs8 = new Observation(NULL,NULL);


    obs1->isValid = true;
    obs1->myXyz.setAt(0, 2.986);
    obs1->myXyz.setAt(1, 2.496);
    obs1->myXyz.setAt(2, 2.928);

    obs2->isValid = true;
    obs2->myXyz.setAt(0, 4.370);
    obs2->myXyz.setAt(1, 5.276);
    obs2->myXyz.setAt(2, 3.079);

    obs3->isValid = true;
    obs3->myXyz.setAt(0, 4.845);
    obs3->myXyz.setAt(1, 6.347);
    obs3->myXyz.setAt(2, 5.817);

    obs4->isValid = true;
    obs4->myXyz.setAt(0, 2.375);
    obs4->myXyz.setAt(1, 1.485);
    obs4->myXyz.setAt(2, 6.589);

    obs5->isValid = true;
    obs5->myXyz.setAt(0, 2.448);
    obs5->myXyz.setAt(1, 1.462);
    obs5->myXyz.setAt(2, 4.205);

    obs6->isValid = true;
    obs6->myXyz.setAt(0, 3.049);
    obs6->myXyz.setAt(1, 2.764);
    obs6->myXyz.setAt(2, 8.050);

    obs7->isValid = true;
    obs7->myXyz.setAt(0, 4.012);
    obs7->myXyz.setAt(1, 4.706);
    obs7->myXyz.setAt(2, 8.082);

    obs8->isValid = true;
    obs8->myXyz.setAt(0, 4.567);
    obs8->myXyz.setAt(1, 5.054);
    obs8->myXyz.setAt(2, 7.141);

    Function *function = new BestFitCircle();

    function->addObservation(obs1,0);
    function->addObservation(obs2,0);
    function->addObservation(obs3,0);
    function->addObservation(obs4,0);
    function->addObservation(obs5,0);
    function->addObservation(obs6,0);
    function->addObservation(obs7,0);
    function->addObservation(obs8,0);

    Circle *myCircle = new Circle();

    QVERIFY2(function->exec(*myCircle), "failed");
}

void OiTestCircle::cleanupTestCase(){
    qDebug()<<"********* Finished testing of OiTestCircle *********";
}
