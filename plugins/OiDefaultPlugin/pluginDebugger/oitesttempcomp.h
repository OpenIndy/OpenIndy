#ifndef OITESTTEMPCOMP_H
#define OITESTTEMPCOMP_H

#include <QObject>
#include <QDebug>
#include "AutoTest.h"

#include "p_simpletemperaturecompensation.h"
#include "p_extendedtemperaturecompensation.h"
#include "p_helmert6param.h"

class OiTestTempComp : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void testTempComp();
    void cleanupTestCase();
};

DECLARE_TEST(OiTestTempComp)
#endif // OITESTTEMPCOMP_H
