#ifndef OITESTCIRCLE_H
#define OITESTCIRCLE_H

#include <QObject>
#include <QDebug>
#include "AutoTest.h"

#include "p_bestfitcircle.h"

class OiTestCircle : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void testLoeslerCircle();
    void cleanupTestCase();
};

DECLARE_TEST(OiTestCircle)
#endif // OITESTCIRCLE_H
