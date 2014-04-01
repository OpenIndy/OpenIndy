#include "oitestcircle.h"

void OiTestCircle::initTestCase(){
    qDebug() << "********* Start testing of OiTestCircle *********";
}

void OiTestCircle::testLoeslerCircle(){

    qDebug() << "--test Loesler circle fit--";

    qDebug() << "set 5 pseudo observations";

    Observation *obs1 = new Observation(NULL,NULL);
    Observation *obs2 = new Observation(NULL,NULL);
    Observation *obs3 = new Observation(NULL,NULL);
    Observation *obs4 = new Observation(NULL,NULL);
    Observation *obs5 = new Observation(NULL,NULL);

    qDebug() << "set coordinates for observation 1";
    obs1->isValid = true;
    obs1->myXyz.setAt(0,0.0);
    obs1->myXyz.setAt(1,1.0);
    obs1->myXyz.setAt(2,0.0);

    qDebug() << "set coordinates for observation 2";
    obs2->isValid = true;
    obs2->myXyz.setAt(0,1.0);
    obs2->myXyz.setAt(1,0.0);
    obs2->myXyz.setAt(2,0.0);

    qDebug() << "set coordinates for observation 3";
    obs3->isValid = true;
    obs3->myXyz.setAt(0,0.0);
    obs3->myXyz.setAt(1,-1.0);
    obs3->myXyz.setAt(2,0.0);

    qDebug() << "set coordinates for observation 4";
    obs4->isValid = true;
    obs4->myXyz.setAt(0,-1.0);
    obs4->myXyz.setAt(1,0.0);
    obs4->myXyz.setAt(2,0.0);

    qDebug() << "set coordinates for observation 5";
    obs4->isValid = true;
    obs4->myXyz.setAt(0,-1.001);
    obs4->myXyz.setAt(1,0.0002);
    obs4->myXyz.setAt(2,0.0001);

    Function *function = new BestFitCircle();

    function->addObservation(obs1,0);
    function->addObservation(obs2,0);
    function->addObservation(obs3,0);
    function->addObservation(obs4,0);
    function->addObservation(obs5,0);

    Circle *myCircle = new Circle();

    QVERIFY2(function->exec(*myCircle), "failed");
}

void OiTestCircle::cleanupTestCase(){
    qDebug()<<"********* Finished testing of OiTestCircle *********";
}
