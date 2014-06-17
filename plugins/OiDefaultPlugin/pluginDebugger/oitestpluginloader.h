#ifndef OITESTPLUGINLOADER_H
#define OITESTPLUGINLOADER_H

#include <QObject>
#include <QDebug>
#include "AutoTest.h"

#include "p_factory.h"

class OiTestPluginLoader : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void testTempComp();
    void cleanupTestCase();
};

DECLARE_TEST(OiTestPluginLoader)

#endif // OITESTPLUGINLOADER_H
