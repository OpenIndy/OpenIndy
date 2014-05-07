#include "oitestYourPluginClass.h"

void OiTestYourPluginClass::initTestCase(){
    qDebug() << "********* Start testing of OiTestTestFit *********";
}

void OiTestYourPluginClass::testTestFit(){

    qDebug() << "test your class here";

    /*e.g
     *
     *YourFitFunction *f = new YourFitFunction();
     *
     *bool checkFunction = f.exec();
     *
     *QVERIFY2(checkFunction == true, "failed");
     */

    double a = 5+5;


    QVERIFY2(a == 5, "failed");

}

void OiTestYourPluginClass::cleanupTestCase(){
    qDebug()<<"********* Finished testing of OiTestTestFit *********";
}
