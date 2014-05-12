#ifndef OITESTYOURPLUGINCLASS_H
#define OITESTYOURPLUGINCLASS_H

#include <QObject>
#include <QDebug>
#include "AutoTest.h"

/*include your plugin classes from
 *from your plugin project ../
 *e.g "yourfitfunction.h"
 */

/*IMPORTANT!!
 *
 *copy all needed dlls to the /bin directory
 *  openIndyLib.dll
 *  lapack.dll
 *  blas.dll
 */

/*!
 * \brief The OiTestYourPluginClass class
 *
 * copy this class and write your own unit test
 */
class OiTestYourPluginClass : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void testTestFit();
    void cleanupTestCase();
};

DECLARE_TEST(OiTestYourPluginClass)
#endif // OITESTYOURPLUGINCLASS_H
