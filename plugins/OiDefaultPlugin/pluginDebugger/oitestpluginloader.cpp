#include "oitestpluginloader.h"

void OiTestPluginLoader::initTestCase()
{
}

void OiTestPluginLoader::testTempComp()
{

    QString path = "D:/SoftDev/Qt/openIndy/bin/debug/plugins/p_defaultPlugind.dll";

    QPluginLoader pluginLoader(path);


       QObject *plugin = pluginLoader.instance();


       if (plugin) {

        OiPlugin *SensorFactory = qobject_cast<OiPlugin *>(plugin);

        SensorFactory->createSensors();

        qDebug() << "check";

       }

       QObject *check = new OiTemplatePlugin();

       qDebug() << "false";

}

void OiTestPluginLoader::cleanupTestCase()
{
}
